Fw_cpu=dlmread("Fw_cpu.txt",";");
Fw=dlmread("Fw.txt",";");
Fw_cpu = Fw_cpu(1:end-1);
Fw = Fw(1:end-1);

max(Fw-Fw_cpu)
min(Fw-Fw_cpu)