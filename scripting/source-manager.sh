#!/bin/bash

# help info
help () {
    echo "Usage:
    -h or --help            This message.
    -S <dir/file> <...>     Adds header to source files where applicable.
    -R <dir/file> <...>     Renames .h files to .hpp if they exist.
    -F <dir/file> <...>     Changes floats to doubles and vice versa."
}

# check for no args (invalid use)
if [ $# = 0 ]
then
    help
    exit 0  # exit script early to avoid parsing rest of script
fi

head () {
    # read header into variable
    head=$(cat header.txt)

    # for each source file with c/c++ relevant extensions
    for file in $(find "$1" -type f -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp") ; do
        name=$(basename -- "$file")
        # check if header already exists
        if grep "$name" -q "$file" ; then
            echo "Header found in '$file', skipping."
        else
            # check if developer has been naughty and fix issues
            if grep "// ${name%.*}" -q "$file" ; then
                echo "Header reporting wrong extension, changing."
                sed -i "s/\/\/ ${name%.*}.*/\/\/ $name/" "$file"
            else
                echo "Adding header to file '$file'."
                # load header into variable after replacing internal name
                HEADER=$(sed -e "s/\/\/ FILE/\/\/ ${name}/" header.txt)
                # prepend header to file ant pipe to tempfile
                echo "$HEADER" | cat - "$file" > tmp
                # overwrite original source file (will remove tmp)
                mv tmp "$file"
            fi
        fi
    done
}

rename () {
    # for each source file with c/c++ relevant extensions
    for file in $(find "$1" -type f -name "*.h") ; do

        # load file into variable
        contents=$(cat "$file")

        name=$(basename -- "$file")
        # check if header already exists
        if grep "// $name" -q "$file" ; then
            echo "Header found in '$file', changing name."

            # get extensionless name
            newname="${name%.*}.hpp"
            # load file as string into NEW after replacing name
            NEW=$(sed -e "s/\/\/ ${name}/\/\/ ${newname}/" "$file")
            # pipe modified contents to new file
            echo "$NEW" > "$newname"
            # remove old file
            rm -f "$file"
        fi
    done
}

floats () {
    # for each source file with c/c++ relevant extensions
    for file in $(find "$1" -type f -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp") ; do
        echo "Swapping all occurances of 'float' and 'double' in file '$file'."

        # replace float with null,
        # then double with float,
        # then null with double.
        sed -i 's/float/\x00/g; s/double/float/g; s/\x00/double/g' "$file"
    done
}

# while there are arguments
while [ $# -gt 0 ] ; do
    case "$1" in
    -h | --help) # get script help
        help
        exit 0
        ;;
    -S) # add header to source files
        if [ $# -gt 1 ] ; then
            while [ $# -gt 1 ] ; do
                head "$2"
                shift
            done
        else
            echo "Too few arguments."
            help
        fi
        exit 0
        ;;
    -R) # rename .h to .hpp
        if [ $# -gt 1 ]
        then
            while [ $# -gt 1 ] ; do
                rename "$2"
                shift
            done
        else
            echo "Too few arguments."
            help
        fi
        exit 0
        ;;
    -F) # change floats to doubles and vice versa.
        if [ $# -gt 1 ]
        then
            while [ $# -gt 1 ] ; do
                floats "$2"
                shift
            done
        else
            echo "Too few arguments."
            help
        fi
        exit 0
        ;;
    *) ;;
    esac
    shift
done
