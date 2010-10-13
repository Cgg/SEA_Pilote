#!/bin/bash
# TeX file generation

# Source info file:
source infos

FILE=${FILE}.tex

echo "% $TITLE - $AUTHOR" > $FILE

# Document class
echo "
\documentclass[twoside]{$DCLASS}
\usepackage{hyperref}
" >> $FILE

# To include pictures
echo "
\usepackage{graphicx}
\usepackage{subfig}
\usepackage{placeins}
" >> $FILE

#Language setting
echo "
% Language settings:
\usepackage[$LANG]{babel}
\usepackage[utf8]{inputenc}
" >> ${FILE}
# If french we have to use T1 fontenc instead of OT1
if [ $LANG="french" ] || [ $LANG="francais" ]
then
	echo "\usepackage[T1]{fontenc}
" >> ${FILE}
fi

# Font setting
echo "
% Font settings:
\usepackage{$FONT}
" >> ${FILE}

# Page layout settings
echo "
\title{${TITLE}}
\author{${AUTHOR}}
% Page layout settings
\usepackage{geometry}
\geometry{
	a4paper,  % 21 x 29,7 cm
	body={170mm,250mm},
	left=20mm, 
	top=30mm,
	headheight=7mm, 
	headsep=4mm,
	marginparsep=4mm,
	marginparwidth=27mm
}
" >> $FILE

# Spacing
echo "
% Spacing:
\usepackage{setspace}
" >> $FILE

# Headers and footers
echo "
% Headers and footers:
\usepackage{fancyhdr}
\pagestyle{fancy}
          \fancyhf{}
          \fancyfoot[LE,RO]{\thepage}
          % Rulers width
          \renewcommand{\footrulewidth}{$FRULE}
          \renewcommand{\headrulewidth}{$HRULE}
\fancyhead[RO,RE]{$AUTHOR}
\fancyfoot[LO,RE]{$TITLE}
" >> ${FILE}

# (Re)define stuff
echo "
% Vars & functs
\newcommand\PIXPATH{$PIXPATH}
\renewcommand{\labelitemi}{$\diamond$}

% Redef m_desc
\newenvironment{m_desc}
{\begin{description}
  \setlength{\itemsep}{4mm}}
{\end{description}}
" >> ${FILE}

# Beggining of document
echo "
% Begining of the document
\begin{document}
" >> ${FILE}

# Main part
	echo "	%Including all the files:" >> $FILE
for i in `ls $TEXPATH`
do
	# Nouvelle page pour chaque partie:
	# \include = \clearpage input{file}  \clearpage
	# echo "\include{${DOCSPATH}/$i}" >> ${3}.tex

	# Pas de nouvelle page pour chaque partie
#	echo "	\input{${TEXPATH}/$i}
#	" >> ${FILE}
	echo "
% Fichier $i
" >> $FILE
#	iconv -t utf-8 "${TEXPATH}/$i" >> $FILE
	cat "$TEXPATH/$i" >> $FILE

done
#	uconv -t utf-8 ${TEXPATH}/*.tex >> temp
#cat temp >> $FILE
#for j in `ls ../src`
#do
#iconv -c -f utf-8 -t latin1 ../src/$j -o docs/src/$j
#done
#iconv -c -f utf-8 -t latin1 $FILE -o ${FILE}.lat1
#mv ${FILE}.lat1 $FILE
# End of document
echo "
% The end
\end{document}
" >> ${FILE}
