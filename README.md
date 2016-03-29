  CONTENT OF THE FILE
  -------------------

 * WHAT IS THIS?
 * DOCUMENTATION
 * INSTALLATION
 * LICENSING
 * KNOWN ISSUES
 * CONTACTS

  WHAT IS THIS?
  -------------

  The HW5 is a software implementation of the AES cryptographic algorithm.

  DOCUMENTATION
  -------------

  "USAGE: ./hw5 operation_name [options]
  operation_name: tablecheck
  				  modprod
  				  keyexpand
                  encrypt
                  decrypt
                  inverse
  [options]     hw5 tablecheck -t=tablefile
    			hw5 modprod -p1=poly1 -p2=poly2
    			hw5 keyexpand -k=key -t=tablefile
    			hw5 encrypt -k=key -t=tablefile [file]
    			hw5 decrypt -k=key -t=tablefile [file]
    			hw5 inverse -p=poly

            However, the order of the operators does not matter.

  INSTALLATION
  ------------

  In order to compile the source code, you should type:

    "make", or "make all", or "make hw5".

  To delete the output files only:

    "make clean_output"

  To delete executable only:

    "make clean_exec"

  To delete .gch, output files and executables, please type:

    "make clean"

  LICENSING
  ---------


  KNOWN ISSUES
  ------------

  In the make file, I used /usr/usc/bin/g++ instead of g++. This was because
  there probably some issue with the path variable. I don't know how to fix them.

  CONTACTS
  --------

     o If you want to inform the developer of this software about any bugs and
     report found errors in the software, please email to <zhanabay@usc.edu>.
