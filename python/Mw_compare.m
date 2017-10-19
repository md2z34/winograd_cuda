Mw_cpu=dlmread("Mw_cpu.txt",";");
Mw=dlmread("Mw.txt",";");
Mw_cpu = Mw_cpu(1:end-1);
Mw = Mw(1:end-1);

max(Mw-Mw_cpu)
min(Mw-Mw_cpu)