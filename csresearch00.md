# Using spack environment with icarus code and hepnos to build new code on csresearch00.fnal.gov

1. setup spack: `source /scratch/gartung/spack/share/spack/setup-env.sh`

2. load the compiler via `spack load gcc@9.3.0`; activate spack environment, the current one available on csresearch is: `spack env activate icaruscode-09_37_02_03-hepnos-0_6_7`

3. clone this repository `git clone https://github.com/HEPonHPC/icarus_filter.git`

4. set up the environment variables: `source envvariable.sh`

5. add an additional path `export FW_SEARCH_PATH=/scratch/gartung/spack/opt/spack/linux-scientific7-x86_64_v2/gcc-9.3.0/icarusalg-09.37.02.01-24uyymplbwvalzi36hbosaumhvlnp4ql/gdml/:$FW_SEARCH_PATH`

6. make a build directory and run cmake in the build directory: `mkdir build && cd build && cmake ../`

7. build the code: `make -j4 all`

8. Update the `FHICL_FILE_PATH`: `export FHICL_FILE_PATH=${BASEDIR}/fcl:${FHICL_FILE_PATH}` where `${BASEDIR}` is the location where this repository has been cloned.

9. Update the `CET_PLUGIN_PATH`: export CET_PLUGIN_PATH=${BUILDDIR}/src/modules:${CET_PLUGIN_PATH} where `${BUILDDIR}` is the build directory
10. 
11. Run the analysis by `lar -c ${DESIRED_ANALYSIS_FHICL}.fcl -n 1 -s /scratch/ssehrish/icaruswf_build/test/prodcorsika_intime_proton_icarus_bnb_20220517T200548_c633562d-c7c5-413d-9d4c-8f0bd8caa2ee.root` by using the desired analysis fhicl file (standalone filter / integrated workflow). 
