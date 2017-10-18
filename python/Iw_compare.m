Iw_cpu=dlmread("Iw_cpu.txt",";");
Iw=dlmread("Iw.txt",";");
Iw_cpu = Iw_cpu(1:end-1);
Iw = Iw(1:end-1);

max(Iw-Iw_cpu)
min(Iw-Iw_cpu)