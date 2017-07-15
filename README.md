# Diff-tool
This is a simple c++ program to diff two files. Its not very well tested or efficient, but works decently to see the differnences between two text files.

The algorithm used is a simple Dynamic programming approach to decide which line of file1 to match with which line of file2. The complexity is O(nm), where n,m are number of lines in files.
