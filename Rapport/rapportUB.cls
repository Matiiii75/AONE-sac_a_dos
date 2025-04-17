\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{rapportUB}[2020/01/10, Style pour rapport, V1]

\newif\if@french\@frenchtrue
\DeclareOption{french}{\@frenchtrue}
\DeclareOption{english}{\@frenchfalse}

\newif\if@chapter\@chapterfalse
\DeclareOption{chapter}{\@chaptertrue}
\DeclareOption{nochapter}{\@chapterfalse}

\newif\if@nologo\@nologofalse
\DeclareOption{nologo}{\@nologotrue}

\DeclareOption{10pt}{\gdef\@pt@size{10pt}}
\DeclareOption{11pt}{\gdef\@pt@size{11pt}}
\DeclareOption{12pt}{\gdef\@pt@size{11pt}}

%\DeclareOption*{}

\ExecuteOptions{french,nochapter,11pt}
\ProcessOptions\relax

\if@chapter
\if@french
\LoadClass[french,a4paper,\@pt@size]{report}
\else
\LoadClass[english,a4paper,\@pt@size]{report}
\fi
\else
\if@french
\LoadClass[french,a4paper,\@pt@size]{article}
\else
\LoadClass[english,a4paper,\@pt@size]{article}
\fi
\fi

\RequirePackage[utf8]{inputenc} 
\RequirePackage[T1]{fontenc}
\RequirePackage{babel}
\RequirePackage{csquotes}
\RequirePackage[lined,ruled,boxed,linesnumbered]{algorithm2e}
\RequirePackage{hyperref}
\RequirePackage{url}
\RequirePackage{amsmath,amsfonts,amsbsy,amssymb,mathabx,amsthm,bbm,bm} 
\RequirePackage[a4paper,top=2cm, bottom=1.5cm, left=2cm, right=2cm, foot=1cm]{geometry}
\RequirePackage{graphicx}
\RequirePackage{array}
\RequirePackage{xcolor}
\RequirePackage{mdframed}
\RequirePackage{ifmtarg}
\RequirePackage{xargs}


\definecolor{blueUB}{RGB}{0,157,224}
\definecolor{marronUB}{RGB}{68,58,49}


\newcommand{\@logo}{logo}

% Tout d'abord je cree les variables utilisees par \maketitle avec leur valeur
% par defaut ...
\newcommand{\@college}{\empty}
\newcommand{\@uf}{\empty}
\newcommand{\@program}{\empty}
\newcommand{\@course}{\empty}
\newcommand{\@academicyear}{\empty}

% ... ensuite je cree les commandes qui permettent a l'utilisateur de modifier
% le contenu de ces variables ...
\newcommand{\college}[1]{\renewcommand{\@college}{\textsf{#1}}}
\newcommand{\uf}[1]{\renewcommand{\@uf}{\textsf{#1}}}
\newcommand{\program}[1]{\renewcommand{\@program}{#1}}
\newcommand{\course}[1]{\renewcommand{\@course}{#1}}
\newcommand{\academicyear}[1]{\renewcommand{\@academicyear}{#1}}

% Je redefini la valeur par defaut de la date, pour ne laisser apparaitre que
% l'annee universitaire en cours, en considerant l'annee universitaire allant
% de septembre (annee-1) a juin (annee).
%\newcounter{@before}
%\setcounter{@before}{\year}
%\newcounter{@after}
%\setcounter{@after}{\year}
%\ifthenelse{\month < 10}%
%{\addtocounter{@before}{-1}}
%{\addtocounter{@after}{1}}

% Je redefinis maintenant la commande \maketitle.
\renewcommand{\maketitle}{
	% Je ne veux pas qu'il y ait de figure ou de table avant le titre qui doit
	% donc etre place en haut de la page ...
	\suppressfloats[t]
	\par
	\begin{center}
		\if@nologo
		\else
		\includegraphics[height=8\baselineskip]{\@logo}
		\fi
		
		\vspace{0.2cm}
		\par
		\textcolor{marronUB}{\Large{\@college}}
		\par
		\vspace{0.1cm}
		\textcolor{marronUB}{\LARGE{{\@uf}}}
		\par
		\vspace{3cm}
		\Huge{\textbf{{\@title}}}
		\par
		\vspace{1cm}
		\LARGE{
			\begin{tabular}[t]{c}%
				\@author
			\end{tabular}
		}
		\par
		\vfill
		% Bottom of the page
		\textcolor{blueUB}{\ \rule{0.9\linewidth}{1pt}}
		\par
		\textcolor{blueUB}{\LARGE{\textbf{\@program}}}
		\par
		%	\vspace{-0.1cm}  \textcolor{blueUB}{\large\the@before--\the@after}
		\vspace{-0.1cm}  \textcolor{blueUB}{\large\@academicyear}
		\par
		\vspace{0.3cm}
		\textcolor{blueUB}{\Large{\textbf{\@course}}}
		\textcolor{blueUB}{\ \rule{0.9\linewidth}{1pt}}
		\par
		\vspace{2cm}
		%\small{\@date}
	\end{center}
	% on ne numérote pas la page
	\thispagestyle{empty}
	% Il faut placer les eventuelles notes de bas de page.  
	\@thanks
	% on passe à la page suivante
	\clearpage
	
	% C'est fini je peux vider les variables utilisees.
	%  \setcounter{@before}{0}
	%  \setcounter{@after}{0}
	\global\let\thanks\relax
	\global\let\@thanks\@empty
	\global\let\maketitle\relax
	\global\let\@maketitle\relax
	\global\let\date\relax
	\global\let\@date\@empty
	\global\let\and\relax
	\global\let\@college\@empty
	\global\let\college\relax
	\global\let\@uf\@empty
	\global\let\uf\relax
	\global\let\@program\@empty
	\global\let\program\relax
	\global\let\@course\@empty
	\global\let\course\relax
	\global\let\@academicyear\@empty
	\global\let\academicyear\relax
}


\usepackage{xparse} % Required for defining commands with multiple optional parameters

\NewDocumentCommand{\engagementPlagiatIA}{m m o o o o}{
	\clearpage
	\begin{center}
		{\LARGE \textcolor{blueUB}{\textbf{Engagement de non plagiat}}}
	\end{center}
	
	
	\vspace{1cm}
	\@ifnotmtarg{#3}{Nous,} #2\@ifnotmtarg{#3}{, #3}{}\@ifnotmtarg{#4}{, #4}{}, \@ifnotmtarg{#3}{nous engageons} fermement à éviter tout acte de plagiat dans ce rapport. 
	
	\medskip
	En signant cet engagement :
	\begin{itemize}
		\item \@ifnotmtarg{#3}{nous nous engageons} à citer correctement toutes les sources utilisées dans mes travaux, en suivant les conventions de citation appropriées.
		
		\item \@ifnotmtarg{#3}{nous nous engageons} à produire un rapport original, évitant tout recours direct ou indirect au travail d'autrui sans attribution adéquate.
		
		\item \@ifnotmtarg{#3}{nous nous engageons} à respecter les droits d'auteur et la propriété intellectuelle, en obtenant les autorisations nécessaires pour utiliser et citer des œuvres protégées par le droit d'auteur.
	\end{itemize}
	
	
	\medskip
	\@ifnotmtarg{#3}{Nous comprenons} que tout acte de plagiat peut entraîner des conséquences disciplinaires. 
	
	\vspace{1cm}
	
	Fait le #1
	
	\vspace{1cm}
	
	\@ifnotmtarg{#3}{\underline{Signatures}}
	
	\hspace{1em}
	\begin{tabular}[t]{c}%
		#2 \\
		\@ifnotmtarg{#3}{#3\\}{}
		\@ifnotmtarg{#4}{#4\\}{}
	\end{tabular}
	
	\vspace{2cm}
	\begin{center}
		{\LARGE \textcolor{blueUB}{\textbf{Engagement à l'égard de l'Intelligence Artificielle}}}
	\end{center}
	
	\vspace{1cm}
	\@ifnotmtarg{#3}{Nous,} #2\@ifnotmtarg{#3}{, #3}{}\@ifnotmtarg{#4}{, #4}{}, \@ifnotmtarg{#3}{nous engageons} à ne pas utiliser d'Intelligence Artificielle générative de texte dans le cadre de la rédaction de ce rapport.
	
	\medskip
	\@ifnotmtarg{#3}{Nous nous engageons} à utiliser d'Intelligence Artificielle générative de code uniquement pour des besoins ponctuels en documentant clairement son utilisation dans le rapport.
	
	\medskip
	\@ifnotmtarg{#3}{Nous comprenons}{Je comprends} que toute violation de cet engagement peut entraîner des conséquences disciplinaires. 
	
	\vspace{1cm}
	
	Le #1
	
	\vspace{1cm}
	
	\@ifnotmtarg{#3}{\underline{Signatures}}
	
	\hspace{1em}
	\begin{tabular}[t]{c}%
		#2 \\
		\@ifnotmtarg{#3}{#3\\}{}
		\@ifnotmtarg{#4}{#4\\}{}
	\end{tabular}
	\thispagestyle{empty}
	\clearpage
}
