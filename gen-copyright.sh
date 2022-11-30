COPYRIGHT=$PWD"/src/includes/copyright"
LOGO=$PWD"/logo.ans"

# Si onefetch est installé, on l'utilise
if [ -x "$(command -v onefetch)" ]; then
  onefetch --ascii-input "$(cat $LOGO)" > $COPYRIGHT 

  # On supprime les deux dernières lignes
  sed -i -e :a -e '$d;N;2,2ba' -e 'P;D' $COPYRIGHT
  #On ajoute un " au début de la première ligne
  sed -i '1s/^/"/' $COPYRIGHT
  #On ajoute un \n\ à la fin de chaque ligne
  sed -i 's/$/\\n\\/' $COPYRIGHT

  #On rajoute une dernière ligne
  echo '' >> $COPYRIGHT

  #On ajoute un " à la fin de la dernière ligne
  sed -i '$s/$/"/' $COPYRIGHT

fi