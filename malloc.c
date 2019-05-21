#include <stdio.h>

typedef struct header{
	struct header *next;
	unsigned size;
} Header;


static Header *morecore(unsigned);
void free_b(void *);


static Header base;
/* last Header used by free_b */
static Header *freep = NULL;


void *malloc_b(unsigned nbytes) {

	Header *prev, *p;
	unsigned nunits;

	nunits = (sizeof(Header) + nbytes -1)/ sizeof(Header) + 1;

	/* no list has been initialize */
	if ((prev = freep) == NULL) {
		base.next = prev = freep = &base;
		base.size = 0;
	}

	for (p = prev->next; ;prev = p, p = p->next) {
		if (p->size >= nunits) {
			if (p->size == nunits){
				prev->next = p->next;
			}else {
				p->size -= nunits;
				p += p->size;
				p->size = nunits;
			}
			freep = prev;
			return (void *) (p+1);
		}

		/* if there is no list initialized or a full circle of the nodes was made*/
		if (p == freep)
			if ((p = morecore(nunits)) == NULL)
				return NULL;
	}
}


#define NALLOC	1024

static Header *morecore(unsigned nu) {
	char *cp, *sbrk(int);
	Header *up;

	nu = (nu > NALLOC) ? nu : NALLOC;
	cp = sbrk(sizeof(Header) *nu);
	if (cp == (char *) -1)
		return NULL;
	up = (Header *) cp;
	up->size = nu;
	/* use free_b to initialize the list */
	free_b((void *) (up+1));
	return freep;
}

void free_b(void *ap) {

	Header *bp;		/* bp: current Header being added to list */
	Header *p;		/* p: previous lower Header than bp */

	/* Set bp to one address less than the user sent */
	bp = (Header *) ap - 1;


	/* adjust p to point to the proper Hearder */
	for (p = freep; !(bp > p && bp < p->next) ; p = p->next){
		if (p >= p->next && (bp > p || bp < p->next))		// last node in list reached and  
			break;											// bp is in the middle of p and its next
	}

	/* if bp is just before the node that p points to */
	if (bp + bp->size == p->next) { /* bp = 1021{size: 2},p = 1{next: 1023}; B-1-1023 */
		bp->size += p->next->size;		/* bp.size = 4  */
		bp->next = p->next->next;		/* bp.next = 1023->next = B 	B-1-1021*/

	}else								/* bp = 1019, p=1{next: 1023}; B-1-1023 */
		bp->next = p->next;				/* bp->next = p->next = 1023	B-1-1019-1023 */


	/* if bp is right after p  */
	if (p + p->size == bp) {		/* bp = 1019{size: 4},p = 1{size: 1018}; B-1-1023 */
		p->size += bp->size;		/* p->size = 1022 */
		p->next = bp->next;			/* p->next = bp->next = 1023  B-1-1023 */
					
	}else							/* bp = 1016, p = 1{size: 1000}; B-1-1023 	1016-1023*/
		p->next = bp;				/* p->next = 1016		B-1-1016-1023 */

	/* set freep to the last Header used for list adjustment */
	freep = p;
}