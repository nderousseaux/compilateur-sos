#!/bin/bash
TEST_FOLDER=$PWD"/tests"
LOG_FILE=$TEST_FOLDER"/tests.log"
SIMULATEUR="spim"
MAIN=$PWD"/SoS"
NB_SUCCESS=0
NB_TESTS=1

main () {
    echo "Démarage des tests ....\n"
    #On intialise le fichier de log
    rm -f "$LOG_FILE"
    touch "$LOG_FILE"

    #On supprime les fichiers temporaire
    rm -rf /tmp//test-sos*

    #Test liminaire
    if ! test_liminaire; then
      fail
      exit 1
    fi

    categories_tests=$1
    nb_categories_tests=$2
    i=1
    for categorie_tests in $categories_tests
    do
        entete_categorie $(basename $categorie_tests) $i $nb_categories_tests
        cd $categorie_tests
        tests=$(find ./* -maxdepth 0 -type d | sort -V)        
        nb_tests=$(find ./* -maxdepth 0 -type d|wc -l|sed 's/ //g')
        j=1
        for test in $tests
        do
            entete_test $test $j $nb_tests
            cd $test
            do_test
            cd ..
            j=$((j+1))
            NB_TESTS=$((NB_TESTS+1))
            echo "\n" >> $LOG_FILE
        done
        cd ..
        i=$((i+1))
    done

    if [ $NB_SUCCESS -eq $NB_TESTS ]; then
      coloredEcho "\nTous les tests ont réussi ! ($NB_SUCCESS/$NB_TESTS)" green
      return 0
    else
      coloredEcho "\nErreurs ! ($NB_SUCCESS/$NB_TESTS)" red
      return 1
    fi
}
test_liminaire(){
  entete_test "compil" 1 1

  #On compile le programme
  echo "\n#### Compilation" >> $LOG_FILE
  echo "#### make clean && make" >> $LOG_FILE
  make clean >> $LOG_FILE 2>&1
  EXITCODE=$?
  
  if [ $EXITCODE -ne 1 ]; then
    make >> $LOG_FILE 2>&1
    EXITCODE=$?
  fi

  echo "==> exitcode: $EXITCODE" >> $LOG_FILE

  if [ $EXITCODE -ne 0 ]; then
    fail
    exit 1
  else
    success
  fi

  return $EXITCODE
}
do_test(){
  #Si il y a un fichier in.* et pas de fichier .out
  if [ -f "in.*" ] && [ ! -f "out.*" ]; then
    coloredEcho "Le fichier out.txt n'existe pas" orange
    return
  else
    #On stocke le fichier out.txt dans une variable
    EXCPECTED=out.txt
  fi

  #On stocke le chemin du fichier assembleur (si il existe)
  if [ -f "in.asm" ]; then
    FILE_ASM=in.asm
  else
    FILE_ASM=/tmp/test-sos.$CATEGORIE_TEST.$TEST.asm.$$
  fi

  OUTPUT=/tmp/test-sos.$CATEGORIE_TEST.$TEST.out.$$

  #Si il y a un fichier qui commence par in
  if [ $(ls in*|wc -l|sed 's/ //g') -gt 0 ]; then

    
    #Si il y a un fichier in-err.sos
    if [ -f "in-err.sos" ]; then
      
      if test_compilation in-err.sos; then
        if test_execution $FILE_ASM; then
          fail
          return
        else
          success
          return
        fi
      else
        success
        return
      fi
    fi

    #Si il y a un fichier in.sos
    if [ -f "in.sos" ]; then
      if ! test_compilation in.sos; then
        fail
        return
      fi
    fi

    #On exécute le fichier compilé, si faux, on arrête le test
    if ! test_execution $FILE_ASM; then
      fail
      return
    fi
     
    
    #On compare le résultat avec le fichier out.txt
    if ! test_sortie $EXCPECTED $OUTPUT; then
      fail
      return
    fi

    #Si le test est passé, on affiche un message de succès
    success

  else
    coloredEcho "Le fichier in* n'existe pas" orange
    return
  fi
}
fail (){
    coloredEcho "KO!\t(Voir le fichier $(basename $LOG_FILE))" red
}
success(){
  coloredEcho "OK!" green
  NB_SUCCESS=$((NB_SUCCESS+1))
}
test_compilation(){
  in=$1
  echo "\n#### Compilation" >> $LOG_FILE
  echo "#### ./SoS -o $FILE_ASM $in" >> $LOG_FILE
  $MAIN -o $FILE_ASM $in >> $LOG_FILE 2>&1
  EXITCODE=$?
  
  if [ $EXITCODE -ne 1 ]; then
    if [ ! -f $FILE_ASM ]; then
      echo "Le fichier $FILE_ASM n'existe pas" >> $LOG_FILE
      EXITCODE=1
    else
    cat $FILE_ASM >> $LOG_FILE
    fi
    echo "\n" >> $LOG_FILE
  fi

  echo "==> exitcode: $EXITCODE" >> $LOG_FILE

  return $EXITCODE
}
test_execution(){
  in=$1

  echo "\n#### Execution" >> $LOG_FILE
  echo "#### $SIMULATEUR -file $in" >> $LOG_FILE
  $SIMULATEUR -file $in 2>&1 >> $OUTPUT
  EXITCODE=$?
  if [ $EXITCODE -eq 0 ]; then

  # On supprime les lignes une par une, jusqu'à trouver le mot "exception.s"

  #Pour chaque ligne du fichier
  while read line
  do
    #On supprime la ligne
    sed -i '1d' $OUTPUT

    #Si la ligne contient "exceptions.s"
    if echo $line | grep -q "exceptions.s"; then
      break
    fi
  done < $OUTPUT

  sed -i -e '$a\' $OUTPUT
  fi
  
  # conserve la sorte dans le log
  cat $OUTPUT >> $LOG_FILE
  echo "==> exitcode: $EXITCODE" >> $LOG_FILE

  return $EXITCODE
}
test_sortie(){
  excpected=$1
  output=$2

  #sinon on vérifie qu'il retourne bien ce qui été attendu
  echo "\n#### Comparaison" >> $LOG_FILE
  echo "#### diff $excpected $output" >> $LOG_FILE

  #Comparaison
  diff $output $excpected >> $LOG_FILE
  EXITCODE=$?
  cat $output >> $LOG_FILE
  echo "==> exitcode: $EXITCODE" >> $LOG_FILE

  return $EXITCODE
}
entete_categorie () {
    CATEGORIE_TEST="$1"
    NUMERO_CATEGORIE_TEST="$2"
    NB_CATEGORIE_TEST="$3"

    #On ne garde qu'après le premier tiret
    CATEGORIE_TEST=$(echo $CATEGORIE_TEST | cut -d'-' -f2-)
    
    coloredEcho "\n------  Test de $CATEGORIE_TEST ($NUMERO_CATEGORIE_TEST/$NB_CATEGORIE_TEST)  ------" blue
    echo "############## $CATEGORIE_TEST ($NUMERO_CATEGORIE_TEST/$NB_CATEGORIE_TEST) ##############" >> $LOG_FILE
}
entete_test () {
    TEST="$1"
    NUMERO_TEST="$2"
    NB_TEST="$3"

    #On ne garde qu'après le premier tiret
    TEST=$(echo $TEST | cut -d'-' -f2-)

    
    printf "Test $TEST\t"
    
    # Si $TEST à moins de 4 caractères, on ajoute une tabulation
    if [ ${#TEST} -lt 3 ]; then
      printf "\t"
    fi

    # Si $TEST à moins de 11 caractères, on ajoute une tabulation
    if [ ${#TEST} -lt 11 ]; then
      printf "\t"
    fi
    printf "($NUMERO_TEST/$NB_TEST)\t........ "    
    echo "-------------- $TEST --------------" >> $LOG_FILE
}
coloredEcho () {
    local exp="$1";
    local color=$2;
     case $(echo $color | tr '[:upper:]' '[:lower:]') in
      black) color=16 ;;
      red) color=5 ;;
      green) color=2 ;;
      yellow) color=3 ;;
      blue) color=4 ;;
      orange) color=1 ;;
      cyan) color=6 ;;
      gray) color=8 ;;
      white|*) color=15 ;; # white or invalid color
     esac
     if [ $COLORIZE -eq 1 ]; then
      tput setaf $color;
      echo "$exp";
      tput sgr0;
    else
      echo "$exp";
    fi
}

if [ -z ${COLORIZE+x} ]; then
  COLORIZE=1
else
  COLORIZE=0
fi

#Si on a des arguments, on les ajoutes à la liste des tests
if [ $# -gt 0 ]; then
  #Pour chaque argument, on ajoute le dossier correspondant à la liste des tests
  for arg in "$@"; do
    TESTS="$TESTS $TEST_FOLDER/$arg"
  done

  NB_TESTS=$#
else
  TESTS=$(find $TEST_FOLDER/* -maxdepth 0 -type d)
  NB_TESTS=$(find $TEST_FOLDER/* -maxdepth 0 -type d|wc -l|sed 's/ //g')
fi

#Pour chaque test, on teste si il existe
for TEST in $TESTS; do
  if [ ! -d $TEST ]; then
    coloredEcho "Le test $(basename $TEST) n'existe pas" red
    exit 1
  fi
done

main "$TESTS" "$NB_TESTS"