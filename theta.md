# Using spack environment with icarus code and hepnos to build new code on Theta

1. Clone this repository `git clone git@github.com:HEPonHPC/icarus_filter.git`

2. Get an interactive node: `qsub -I -n 1 -t 01:00 --attrs enable_ssh=1 -q debug-cache-quad -A HEP_on_HPC`

3. Change the cray programming environment to gnu : `module swap PrgEnv-intel PrgEnv-gnu`

4. `export ICARUSFILTER_SRC=<path to cloned repository>` 

5. make a build directory for out of source build and cd into it. `export ICARUSFILTER_BUILD=<path to build directory>
&& cd ${ICARUSFILTER_BUILD}` 

6. Load the system provided modules `gcc@9.3.0` and `cray-mpich` via `module load gcc/9.3.0 && module load
cray-mpich/7.7.14`

7. Setup spack: `source /projects/HEP_on_HPC/icaruscode/spack/share/spack/setup-env.sh`

8. Load the compiler via `spack load gcc@9.3.0`; activate spack environment, the current one available on csresearch is:
`spack env activate icaruscode-9-37-02-03-vecmt-hepnos-0_6_5`

9. Set up the environment variables: `source ${ICARUSFILTER_SRC}/envvariable.sh`

10. `cd ${ICARUSFILTER_BUILD} && cmake ${ICARUSFILTER_SRC}`

11. build the code: `make -j4 all`

12. Run `lar -c new_intime_g4_icarus_sce_overburden_lite.fcl -n 5 -s
/projects/HEP_on_HPC/thetaprodcorsika_proton_intime_icarus_bnb_sce_on_ovb/prodcorsika_intime_proton_icarus_bnb_20220517T200548_c633562d-c7c5-413d-9d4c-8f0bd8caa2ee.root -o 210722-g4-exception.out`
