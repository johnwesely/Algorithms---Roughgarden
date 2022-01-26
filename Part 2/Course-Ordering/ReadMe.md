prints valid course ordering of curriculum passed in file argv[1]     
curriculum is formatted in format [[v1, j1], [v2, j2], ...]   
where j1 is a prerequisite for v1, v2 is a prerequisite for j2 etc.   
sample input: "[[1,0],[2,0],[3,1],[3,2]]"   
sample output "0, 2, 1, 3"   
