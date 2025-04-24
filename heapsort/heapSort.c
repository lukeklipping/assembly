/* To compile this program:
 *
 *   gcc cs3210_s2025_pa1_heapsort.c -o cs3210_s2025_pa1_heapsort

 * Change SIZE and recompile to run on arrays of different sizes.
 */

#define SIZE 512

/* For this assignment, you'll be implementing heapsort in the LEG subset of
 * the ARMv8 ISA.  Heapsort is an O(n lg n) comparison sort which you should
 * all be familiar with from your data structures course.
 *
 * Regardless of how comfortable you may be with heapsort, or with how well
 * you may comprehend this C implementation, we want to stress here that you
 * do not have to understand this algorithm!  You don't even have to try to
 * understand it.  All that you have to do is, essentially, be the compiler
 * and translate this C code into LEGv8.  This is not to discourage
 * understanding.  Understanding is a good and useful thing.  We are simply
 * pointing out that you don't need to understand the algorithm in order to
 * implement your solution.  It's not rare that a route of expedience is
 * optimal.
 *
 * See the comments and code below for details on exactly what you must
 * implement.
 */
#include <stdio.h>
#include <stdint.h>

/* a: address of array of unsigned 64-bit int
 * s: number of elements in a
 *
 * Initializes the contents of a with the numbers 0 through (s - 1).  The
 * array is initialized with even number to the left in descending order, and
 * odd number on the right in ascending order.
 */
void fill(uint64_t *a, uint64_t s)
{
    uint64_t i = 0;

    if (s >= 2)
    {
        for (; i < s / 2; i++)
        {
            a[((s + 1) / 2) - (i + 1)] = i * 2;
            a[((s + 1) / 2) + i] = i * 2 + 1;
        }
    }

    if (s & 1)
    {
        a[0] = s - 1;
    }
}

/* a: address of an unsigned 64-bit int
 * b: address of an unsigned 64-bit int
 *
 * Swaps the values addressed by a and b.
 */
void swap(uint64_t *a, uint64_t *b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

/* a: address of array of unsigned 64-bit int
 * s: number of elements in a
 * i: an index in a
 *
 * i is the index of the root of a subtree within a.  The children of i are
 * maxheaps.  If the tree rooted at i is not a maxheap, swap the value at i
 * with that of its largest child and repeat this process at the child node
 * until the value origionally at i reaches a position such that i is the
 * root of a maxheap.
 */
void percolate_down(uint64_t *a, uint64_t s, uint64_t i)
{
    uint64_t child;

    for (child = (2 * i) + 1; child < s; i = child, child = (2 * i) + 1)
    {
        if (child + 1 < s && a[child] < a[child + 1])
        {
            child++;
        }
        if (a[i] < a[child])
        {
            swap(a + i, a + child);
        }
    }
}

/* a: address of array of unsigned 64-bit int
 * s: number of elements in a
 *
 * Arrange the elements of a such that a is a maxheap wherein the children of
 * each index, i, are in indices((i * 2) + 1) and ((i * 2) + 2).
 */
void heapify(uint64_t *a, uint64_t s)
{
    uint64_t i;

    for (i = (s + 1) / 2; i; i--)
    {
        percolate_down(a, s, i);
    }
    percolate_down(a, s, 0);
}

/* a: address of array of unsigned 64-bit int
 * s: number of elements in a
 *
 * Sorts elements of a using the heapsort algorithm.
 */
void heapsort(uint64_t *a, uint64_t s)
{
    uint64_t i;

    heapify(a, s);

    for (i = 1; i < s; i++)
    {
        swap(a + 0, a + (s - i));
        percolate_down(a, s - i, 0);
    }
}

/* Your main function should allocate space for an array, call fill to fill
 * it with numbers, and then call heapsort to sort it.  Use the HALT emulator
 * instruction to see the memory contents and confirm that your functions
 * work.  You may choose any array size you like (up to the default limit of
 * memory, 4096 bytes or 512 8-byte integers).
 *
 * After completing all of the above, HALT the emulator to force a core dump
 * so that you (and the TAs) can examine the contents of memory.
 *
 * You must implement all functions described above.  You are acting as the
 * compiler here; you are not granted creative freedom to refactor these
 * functions to your liking.  You must adhere to ARMv8 calling conventions;
 * in particular, you must correctly use the stack when calling and
 * implementing procedures, and a procedure may not "know" anything that it
 * was not explicitly informed of by way of its parameters!  For example:
 * Even though you--the programmer--know that no other procedure uses X22,
 * you still must save it before you use it and restore it when you are
 * finished with it.  Imagine that, instead of you writing all of these
 * procedures, each of them is written by a different person, but none of you
 * are permitted to communicate with each other in any way.  The only thing
 * each of you has is this specification.  When we put all of your procedures
 * together, your program should work, but the only way that will be possible
 * is if you fully adhere to convention.
 *
 * You may work alone or with a single partner on this assignment.
 */
int main(int argc, char *argv[])
{
    /* In your LEGv8 program, main will not be a procedure.  Control will begin
     * at the top of the file, so you should think of that as main.  If control
     * reaches the end of the file, the program will exit, which you may think
     * of as leaving main.
     */
    uint64_t a[SIZE];

    fill(a, SIZE);

    heapsort(a, SIZE);

    /*
    int i;
    for (i = 0; i < SIZE; i++) {
      // Depending on your compiler, you may need %lu instead of %llu
      printf("%llu\t", a[i]);
    }
    printf("\n");
    */

    return 0;
}