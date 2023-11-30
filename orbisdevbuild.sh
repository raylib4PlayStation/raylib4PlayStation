# check-orbisdev.sh by Franciso Javier Trujillo Mata (fjtrujy@gmail.com)

## Check if $ORBISDEV is set.
if test ! $ORBISDEV; then { echo "ERROR: Set \$ORBISDEV before continuing."; exit 1; } fi

## Check for the $ORBISDEV directory.
ls -ld $ORBISDEV 1> /dev/null || mkdir -p $ORBISDEV 1> /dev/null || { echo "ERROR: Create $ORBISDEV before continuing."; exit 1; }

## Check for $ORBISDEV write permission.
touch $ORBISDEV/test.tmp 1> /dev/null || { echo "ERROR: Grant write permissions for $ORBISDEV before continuing."; exit 1; }

## Check for $ORBISDEV/bin in the path.
echo $PATH | grep $ORBISDEV/bin 1> /dev/null || { echo "ERROR: Add $ORBISDEV/bin to your path before continuing."; exit 1; }

wget https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
tar xfz 5.0.tar.gz
cd raylib-5.0
patch --strip=1 --input=../PlayStation_raylib.patch
cd src
make PLATFORM=PLATFORM_ORBIS
make PLATFORM=PLATFORM_ORBIS install
