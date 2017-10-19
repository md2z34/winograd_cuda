Ow_cpu=dlmread("Ow_cpu.txt",";");
Ow=dlmread("Ow.txt",";");
Ow_cpu = Ow_cpu(1:end-1);
Ow = Ow(1:end-1);

max(Ow-Ow_cpu)
min(Ow-Ow_cpu)