# shellcheck disable=SC2046
docker run --platform linux/arm/v7 -v ${PWD}:/tmp/build -w /tmp/build beaglebone ./build.sh