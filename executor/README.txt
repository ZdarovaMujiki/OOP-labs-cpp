Workflow Executor v1.1

simple computing scheme, consisting predefined set of computing blocks and links between them

available blocks:
1. readfile <filename> - read text file and save it in memory
2. writefile <filename> - write text in file
3. grep <word> - choose lines from text, containing <word>
4. sort - lexicographic sort
5. replace <word1> <word2> - replace <word1> by <word2>
6. dump <filename> - save text in file

workflow format:
desc
id1 = block1
id2 = block2
…
idN = blockN
csed
idA -> idB -> idC -> … -> idZ
* desc, csed - keywords
* id1, ..., idN - non-negative unique integers
* block1, ..., blockN - blocks with arguments
* idA, idB, ..., idZ - integers from id1, ..., idN can repeat
* -> - keyword, links between computing nodes

example:
desc
1 = replace abracadabra cadabraabra
2 = grep braab
3 = sort
0 = readfile in.txt
5 = writefile out.txt
csed
0 -> 1 -> 2 -> 3 -> 5

launch:
executor.exe workflow.txt
executor.exe workflow.txt -i <input> -o <output>


