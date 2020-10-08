## OpenCL 2.1 Installation (Intel)

tested version: **20.39.17972**

```
mkdir -p ~/intel && cd ~/intel
wget https://github.com/intel/compute-runtime/releases/download/20.39.17972/intel-gmmlib_20.2.5_amd64.deb
wget https://github.com/intel/intel-graphics-compiler/releases/download/igc-1.0.5064/intel-igc-core_1.0.5064_amd64.deb
wget https://github.com/intel/intel-graphics-compiler/releases/download/igc-1.0.5064/intel-igc-opencl_1.0.5064_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/20.39.17972/intel-opencl_20.39.17972_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/20.39.17972/intel-ocloc_20.39.17972_amd64.deb
wget https://github.com/intel/compute-runtime/releases/download/20.39.17972/intel-level-zero-gpu_1.0.17972_amd64.deb
sudo dpkg -i *.deb
```
