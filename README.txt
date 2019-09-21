The structure that I used to keep track of commands was  a linked list. I similarly used a linked list to keep track of all background processes.
I use time of day to calculate run time of all non background processes. For the background processes I use user time and cpu time.
I tested my programs using an input file and tested all of the functionality, although I did not try all of the edge cases.

    **I had to re-write my entire project when I was ready to submit, because as it turns out, depending on where you handle your inputs, the stdin buffer
    does not update across child and parent processes. So while that is fine when the user is inputting the values (as I was doing while
    writing the project), when the stdin is filled from the get go with a file, the lines that the child reads are re-read by the parent.
    I handled most of the work in the child processes until I realized this error. So I was able to move all of the input handling and linked list
    updating outside of the child processes and into the parent. This proves a much more elegant way to do many of the things required. Not only
    is this a great sob story, it also, I believe, displays how well this project forced me to really think about concurrency and parent child processes.**
