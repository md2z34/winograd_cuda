sliceI_cpu=dlmread("sliceI_cpu.txt",";");
sliceI=dlmread("sliceI.txt",";");
sliceI_cpu = sliceI_cpu(1:16384);
sliceI = sliceI(1:16384);

sliceI_mat = zeros(32,4,4,32);
sliceI_cpu_mat = zeros(32,4,4,32);
for a=1:32,
  for b=1:4,
    for c=1:4,
      for d=1:32,
          sliceI_mat(a,b,c,d) = sliceI((a-1)*4*4*32 + (b-1)*4*32 + (c-1)*32 + d);
          sliceI_cpu_mat(a,b,c,d) = sliceI_cpu((a-1)*4*4*32 + (b-1)*4*32 + (c-1)*32 + d);
      end
     end
  end
end
max(sliceI-sliceI_cpu)
min(sliceI-sliceI_cpu)
