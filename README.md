# LoboSearch
## Description
Reads in graph file passed in by user then calculates pagerank of each node. Each node acts as a page and contains a URL that is a file in users directory. Files are opened and each word inside the files is inserted into a hashtable, only alphabetical strings are inserted everything else is tossed out. User than passes word they want to look for in second argument and programs returns the top 5 nodes (by pages pagerank) that contain that key. Syntax in how user capitalizes key they look for does not matter. **Graph and files inside graph _need_ to be inside of users directory**. 

## example
./query graph.txt computer ./query graph.txt COmPuTeR , **these two return the same thing**.
