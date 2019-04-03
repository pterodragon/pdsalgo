build_type="Debug"
build_tests=""
build_test_extra=""

third_party_dir=$(pwd)/third_party
while getopts "rtp:" o; do
    case "${o}" in
        r) # release build
            build_type="Release"
            ;;
        t)
            build_tests="-DBUILD_TESTS=ON"
            build_test_extra="-DCMAKE_INCLUDE_PATH:PATH=$third_party_dir/cxx-prettyprint"
            ;;
        p)
            temp=${OPTARG}
            ;;

    esac
done
shift $((OPTIND-1))

export CXX="clang++"
export CC="clang"

cmake -H. -Bpdsalgo_build -DCMAKE_INSTALL_PREFIX:PATH=install -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=$build_type $build_tests $build_test_extra
cmake --build pdsalgo_build --config $build_type --target install -- -j 8
