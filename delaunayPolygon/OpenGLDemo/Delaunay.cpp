/*****************************************************************************/
/*                                                                           */
/*  maketriangle()   Create a new triangle with orientation zero.            */
/*                                                                           */
/*****************************************************************************/


void maketriangle(struct mesh *m, struct behavior *b, struct otri *newotri)
{
    int i;

    newotri->tri = (triangle *) poolalloc(&m->triangles);
    /* Initialize the three adjoining triangles to be "outer space". */
    newotri->tri[0] = (triangle) m->dummytri;
    newotri->tri[1] = (triangle) m->dummytri;
    newotri->tri[2] = (triangle) m->dummytri;
    /* Three NULL vertices. */
    newotri->tri[3] = (triangle) NULL;
    newotri->tri[4] = (triangle) NULL;
    newotri->tri[5] = (triangle) NULL;
    if (b->usesegments) {
        /* Initialize the three adjoining subsegments to be the omnipresent */
        /*   subsegment.                                                    */
        newotri->tri[6] = (triangle) m->dummysub;
        newotri->tri[7] = (triangle) m->dummysub;
        newotri->tri[8] = (triangle) m->dummysub;
    }
    for (i = 0; i < m->eextras; i++) {
        setelemattribute(*newotri, i, 0.0);
    }
    if (b->vararea) {
        setareabound(*newotri, -1.0);
    }

    newotri->orient = 0;
}



/********* Incremental Delaunay triangulation begins here            *********/
/**                                                                         **/
/**                                                                         **/

/*****************************************************************************/
/*                                                                           */
/*  boundingbox()   Form an "infinite" bounding triangle to insert vertices  */
/*                  into.                                                    */
/*                                                                           */
/*  The vertices at "infinity" are assigned finite coordinates, which are    */
/*  used by the point location routines, but (mostly) ignored by the         */
/*  Delaunay edge flip routines.                                             */
/*                                                                           */
/*****************************************************************************/


void boundingbox(struct mesh *m, struct behavior *b)
{
    struct otri inftri;          /* Handle for the triangular bounding box. */
    REAL width;

    if (b->verbose) {
        printf("  Creating triangular bounding box.\n");
    }
    /* Find the width (or height, whichever is larger) of the triangulation. */
    width = m->xmax - m->xmin;
    if (m->ymax - m->ymin > width) {
        width = m->ymax - m->ymin;
    }
    if (width == 0.0) {
        width = 1.0;
    }
    /* Create the vertices of the bounding box. */
    m->infvertex1 = (vertex) trimalloc(m->vertices.itembytes);
    m->infvertex2 = (vertex) trimalloc(m->vertices.itembytes);
    m->infvertex3 = (vertex) trimalloc(m->vertices.itembytes);
    m->infvertex1[0] = m->xmin - 50.0 * width;
    m->infvertex1[1] = m->ymin - 40.0 * width;
    m->infvertex2[0] = m->xmax + 50.0 * width;
    m->infvertex2[1] = m->ymin - 40.0 * width;
    m->infvertex3[0] = 0.5 * (m->xmin + m->xmax);
    m->infvertex3[1] = m->ymax + 60.0 * width;

    /* Create the bounding box. */
    maketriangle(m, b, &inftri);
    setorg(inftri, m->infvertex1);
    setdest(inftri, m->infvertex2);
    setapex(inftri, m->infvertex3);
    /* Link dummytri to the bounding box so we can always find an */
    /*   edge to begin searching (point location) from.           */
    m->dummytri[0] = (triangle) inftri.tri;
    if (b->verbose > 2) {
        printf("  Creating ");
        printtriangle(m, b, &inftri);
    }
}

#endif /* not REDUCED */

/*****************************************************************************/
/*                                                                           */
/*  removebox()   Remove the "infinite" bounding triangle, setting boundary  */
/*                markers as appropriate.                                    */
/*                                                                           */
/*  The triangular bounding box has three boundary triangles (one for each   */
/*  side of the bounding box), and a bunch of triangles fanning out from     */
/*  the three bounding box vertices (one triangle for each edge of the       */
/*  convex hull of the inner mesh).  This routine removes these triangles.   */
/*                                                                           */
/*  Returns the number of edges on the convex hull of the triangulation.     */
/*                                                                           */
/*****************************************************************************/


long removebox(struct mesh *m, struct behavior *b)
{
    struct otri deadtriangle;
    struct otri searchedge;
    struct otri checkedge;
    struct otri nextedge, finaledge, dissolveedge;
    vertex markorg;
    long hullsize;
    triangle ptr;                         /* Temporary variable used by sym(). */

    if (b->verbose) {
        printf("  Removing triangular bounding box.\n");
    }
    /* Find a boundary triangle. */
    nextedge.tri = m->dummytri;
    nextedge.orient = 0;
    symself(nextedge);
    /* Mark a place to stop. */
    lprev(nextedge, finaledge);
    lnextself(nextedge);
    symself(nextedge);
    /* Find a triangle (on the boundary of the vertex set) that isn't */
    /*   a bounding box triangle.                                     */
    lprev(nextedge, searchedge);
    symself(searchedge);
    /* Check whether nextedge is another boundary triangle */
    /*   adjacent to the first one.                        */
    lnext(nextedge, checkedge);
    symself(checkedge);
    if (checkedge.tri == m->dummytri) {
        /* Go on to the next triangle.  There are only three boundary   */
        /*   triangles, and this next triangle cannot be the third one, */
        /*   so it's safe to stop here.                                 */
        lprevself(searchedge);
        symself(searchedge);
    }
    /* Find a new boundary edge to search from, as the current search */
    /*   edge lies on a bounding box triangle and will be deleted.    */
    m->dummytri[0] = encode(searchedge);
    hullsize = -2l;
    while (!otriequal(nextedge, finaledge)) {
        hullsize++;
        lprev(nextedge, dissolveedge);
        symself(dissolveedge);
        /* If not using a PSLG, the vertices should be marked now. */
        /*   (If using a PSLG, markhull() will do the job.)        */
        if (!b->poly) {
            /* Be careful!  One must check for the case where all the input     */
            /*   vertices are collinear, and thus all the triangles are part of */
            /*   the bounding box.  Otherwise, the setvertexmark() call below   */
            /*   will cause a bad pointer reference.                            */
            if (dissolveedge.tri != m->dummytri) {
                org(dissolveedge, markorg);
                if (vertexmark(markorg) == 0) {
                    setvertexmark(markorg, 1);
                }
            }
        }
        /* Disconnect the bounding box triangle from the mesh triangle. */
        dissolve(dissolveedge);
        lnext(nextedge, deadtriangle);
        sym(deadtriangle, nextedge);
        /* Get rid of the bounding box triangle. */
        triangledealloc(m, deadtriangle.tri);
        /* Do we need to turn the corner? */
        if (nextedge.tri == m->dummytri) {
            /* Turn the corner. */
            otricopy(dissolveedge, nextedge);
        }
    }
    triangledealloc(m, finaledge.tri);

    trifree((VOID *) m->infvertex1);  /* Deallocate the bounding box vertices. */
    trifree((VOID *) m->infvertex2);
    trifree((VOID *) m->infvertex3);

    return hullsize;
}



/*****************************************************************************/
/*                                                                           */
/*  incrementaldelaunay()   Form a Delaunay triangulation by incrementally   */
/*                          inserting vertices.                              */
/*                                                                           */
/*  Returns the number of edges on the convex hull of the triangulation.     */
/*                                                                           */
/*****************************************************************************/
long incrementaldelaunay(struct mesh *m, struct behavior *b)
{
    struct otri starttri;
    vertex vertexloop;

    /* Create a triangular bounding box. */
    boundingbox(m, b);
    if (b->verbose) {
        printf("  Incrementally inserting vertices.\n");
    }
    traversalinit(&m->vertices);
    vertexloop = vertextraverse(m);
    while (vertexloop != (vertex) NULL) {
        starttri.tri = m->dummytri;
        if (insertvertex(m, b, vertexloop, &starttri, (struct osub *) NULL, 0, 0)
            == DUPLICATEVERTEX) {
                if (!b->quiet) {
                    printf(
                        "Warning:  A duplicate vertex at (%.12g, %.12g) appeared and was ignored.\n",
                        vertexloop[0], vertexloop[1]);
                }
                setvertextype(vertexloop, UNDEADVERTEX);
                m->undeads++;
        }
        vertexloop = vertextraverse(m);
    }
    /* Remove the bounding box. */
    return removebox(m, b);
}



/**                                                                         **/
/**                                                                         **/
/********* Incremental Delaunay triangulation ends here              *********/