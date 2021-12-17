# Operating System Programming Assignment 2
## Tanmay Garg CS20BTECH11063

- There are 4 files in the folder
    - Assgn2Src_CS20BTECH11063.c
    - Assgn2Readme_CS20BTECH11063.txt
    - Assgn2Report_CS20BTECH11063.pdf
    - input.txt

- To compile and run the program enter the following commands in terminal or powershell window

```
$ gcc -o final_exec Assgn2Src_CS20BTECH11063.c
$ ./final_exec
```
- To run only the input/output part of the assignment, comment the second ``` main ``` function
- To run the report part of the assignment, comment the first ``` main ``` function, then comment and uncomment required parts for part a and b (mentioned in the source code)
- To run the report part for instance 100 time to test for a partcicular size of array and number of threads then run the following code in bash
```
$ for i in {1..100} ; do ./final_exec ; done
```