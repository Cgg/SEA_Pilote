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
if [ $FONT != "default" ]
then
    echo "
% Font settings:
\usepackage{$FONT}
" >> ${FILE}
fi

# Page layout settings
echo "
\title{${TITLE}}
\author{${AUTHOR}}
% Page layout settings
\usepackage{geometry}
\geometry{
	a4paper,  % 21 x 29,7 cm
	body={140mm,230mm},
	left=35mm, 
	top=30mm,
	headheight=7mm, 
	headsep=4mm,
	marginparsep=4mm,
	marginparwidth=27mm
}
" >> $FILE

# Listings

echo "
 \usepackage{minted}
\newminted{sql}{%
	linenos=true,%
	fontsize=\footnotesize,%
	frame=single,%
	framesep=9pt,%
	xrightmargin=8mm,%
	xleftmargin=15mm,%
	resetmargins=true,%
}%
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
\newcommand\SRCPATH{$SRCPATH}
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
for i in `ls ${TEXPATH}/*.tex`
do
	echo "
% Fichier $i
" >> $FILE
	if [ `file -i "$i" | grep utf-8 | wc -l` == 1 ]
    then
        cat "$i" >> $FILE
        echo "File $i included"
    else
	    iconv -flatin1 -tutf-8 "$i" >> $FILE
        echo "File $i converted to utf-8 and included"
    fi

done

# End of document
echo "
% The end
\end{document}
" >> ${FILE}
