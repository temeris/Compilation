#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_lexical.h"
#include "symboles.h"
#include "premiers.h"
#include "suivants.h"
#include "analyseur_syntaxique.h"
#include "tabsymboles.h"

char yytext[100];
char nom[100];
char valeur[100];
int analyse_lex = 0;
int analyse_synt = 0;

FILE *yyin;
int uniteCourante;


void erreur_syntaxe(void)
{
	printf("Expression incorrecte !\n");
	exit(-1);
}

n_prog programme(void)
{
	n_prog *$$ = NULL;
	n_instr *$1 = NULL;
	n_l_dec *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	
	if(est_premier(_optDecVariables_, uniteCourante) || est_premier(_listeDecFonctions_, uniteCourante ) ||
	est_suivant(_programme_,uniteCourante))
	{
		$1 = optDecVariables();
		$2 = listeDecFonctions();
		$$ = cree_n_prog($1, $2);
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec optDecVariables(void)
{
	n_l_dec *$$ = NULL;
	n_l_dec *$1 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_listeDecVariables_, uniteCourante ))
	{
		$1 = listeDecVariables();
		if(uniteCourante==POINT_VIRGULE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			$$ = cree_n_l_dec($1);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else if(est_suivant(_optDecVariables_,uniteCourante)) 
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec listeDecVariables(void)
{
	n_l_dec *$$ = NULL;
	n_dec *$1 = NULL;
	n_l_dec *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_declarationVariable_, uniteCourante ))
	{
		$1 = declarationVariable();
		$2 = listeDecVariablesBis();
		$$ = cree_n_l_dec($1, $2);
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec listeDecVariablesBis(void)
{
	n_l_dec *$$ = NULL;
	n_dec *$1 = NULL;
	n_l_dec *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==VIRGULE){
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_declarationVariable_,uniteCourante))
		{
			$1 = declarationVariable();
			$2 = listeDecVariablesBis();
			$$ = cree_n_l_dec($1, $2);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else if(est_suivant(_listeDecVariablesBis_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_dec declarationVariable(void)
{
	n_dec *$$ = NULL;
	n_dec *$1 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ENTIER)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==ID_VAR)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_optTailleTableau_,uniteCourante) || est_suivant(_declarationVariable_,uniteCourante))
			{
				$1 = optTailleTableau();
				$$ = cree_n_dec_var($1);
				affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

int optTailleTableau(void)
{	
	int $$ = -1;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==CROCHET_OUVRANT)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==NOMBRE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			$$ = atoi(valeur);
			uniteCourante=yylex();
			if(uniteCourante==CROCHET_FERMANT)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
		}
	}
	else if (est_suivant(_optTailleTableau_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec listeDecFonctions(void)
{
	n_l_dec *$$ = NULL;
	n_dec *$1 = NULL;
	n_l_dec *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_declarationFonction_, uniteCourante ))
	{
		$1 = declarationFonction();
		$2 = listeDecFonctions();
		$$ = cree_n_l_dec($1, $2);
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else if (est_suivant(_listeDecFonctions_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}
 
n_dec declarationFonction(void)
{
	n_dec *$$ = NULL;
	n_l_dec *$1 = NULL;
	n_dec *$2 = NULL;
	n_instr *$3 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ID_FCT)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_listeParam_, uniteCourante ))
		{
			$1 = listeParam();
			$2 = optDecVariables();
			$3 = instructionBloc();
			$$ = cree_n_dec_fonc($1, $2, $3);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec listeParam(void)
{
	n_l_dec *$1 = NULL;
	 
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==PARENTHESE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_optListeDecVariables_,uniteCourante) || est_suivant(_optListeDecVariables_,uniteCourante))
		{
			$1 = optListeDecVariables();
			if (uniteCourante==PARENTHESE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return $1;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_dec optListeDecVariables(void)
{
	n_l_dec *$1 = NULL;
	
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_listeDecVariables_,uniteCourante))
	{
		$1 =listeDecVariables();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if (est_suivant(_optListeDecVariables_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_instr instruction(void)
{
	n_instr *$1 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_instructionAffect_,uniteCourante))
	{
		$1 = instructionAffect();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionBloc_,uniteCourante))
	{
		$1 = instructionBloc();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	
	else if(est_premier(_instructionSi_,uniteCourante))
	{
		$1 = instructionSi();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionTantque_,uniteCourante))
	{
		$1 = instructionTantque();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionAppel_,uniteCourante))
	{
		$1 = instructionAppel();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionRetour_,uniteCourante))
	{
		$1 = instructionRetour();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionEcriture_,uniteCourante))
	{
		$1 = instructionEcriture();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else if(est_premier(_instructionFaire_,uniteCourante))
	{
		$1 = instructionFaire();
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_instr instructionAffect(void)
{
	n_instr *$$ = NULL;
	n_var *$1 = NULL;
	n_exp *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_var_,uniteCourante))
	{
		$1 = var();
		if (uniteCourante==EGAL)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_expression_,uniteCourante))
			{
				$2 = expression();
				if (uniteCourante==POINT_VIRGULE)
				{
					nom_token(uniteCourante,nom,valeur);
					affiche_element(nom,valeur,1);
					uniteCourante=yylex();
					$$ = cree_n_instr_affect($1, $2);
					affiche_balise_fermante(__FUNCTION__,1);
					return $$;
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_instr instructionBloc(void)
{
	n_instr *$$ = NULL;
	n_l_instr *$1 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ACCOLADE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_listeInstructions_,uniteCourante) || est_suivant(_instructionBloc_,uniteCourante))
		{
			$1 = listeInstructions();
			if (uniteCourante==ACCOLADE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				$$ = cree_n_instr_bloc($1);
				affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_instr listeInstructions(void)
{
	n_l_instr *$$ = NULL;
	n_instr *$1 = NULL;
	n_l_instr *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_instruction_,uniteCourante))
	{
		$1 = instruction();
		$2 = listeInstructions();
		$$ = cree_n_l_instr($1, $2);
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else if (est_suivant(_listeInstructions_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_instr instructionSi(void)
{
	n_instr *$$ = NULL;
	n_exp *$1 = NULL;
	n_instr *$2 = NULL;
	n_instr *$3 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==SI)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_expression_,uniteCourante)) 
		{
			$1 = expression();
			if (uniteCourante==ALORS)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				if (est_premier(_instructionBloc_,uniteCourante)) 
				{
					$2 = instructionBloc();
					$3 = optSinon();
					$$ = cree_n_instr_si($1, $2, $3);
					affiche_balise_fermante(__FUNCTION__,1);
					return $$;
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);	
}

n_instr optSinon(void)
{
	n_instr *$1 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==SINON)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_instructionBloc_,uniteCourante))
		{
			$1 = instructionBloc();
			affiche_balise_fermante(__FUNCTION__,1);
			return $1;
		}
	}
	else if(est_suivant(_optSinon_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return $1;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_instr instructionTantque(void)
{
	n_instr *$$ = NULL;
	n_exp *$1 = NULL;
	n_instr *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==TANTQUE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			$1 = expression();
			if (uniteCourante==FAIRE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				if(est_premier(_instructionBloc_,uniteCourante))
				{
					$2 = instructionBloc();
					$$ = cree_n_instr_tantque($1, $2);
					affiche_balise_fermante(__FUNCTION__,1);
					return $$;
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}		

n_instr instructionAppel(void)
{
	n_instr *$$ = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_appelFct_,uniteCourante))
	{
		$1 = appelFct();
		if (uniteCourante==POINT_VIRGULE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			$$ = cree_n_instr_appel($1);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_instr instructionRetour(void)
{
	n_instr *$$ = NULL;
	n_exp *$1 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==RETOUR)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			$1 = expression();
			if (uniteCourante==POINT_VIRGULE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				$$ = cree_n_instr_retour($1);
				affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_instr instructionEcriture(void)
{
	n_instr *$$ = NULL;
	n_exp *$1 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ECRIRE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (uniteCourante==PARENTHESE_OUVRANTE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_expression_,uniteCourante))
			{
				$1 = expression();
				if (uniteCourante==PARENTHESE_FERMANTE)
				{
					nom_token(uniteCourante,nom,valeur);
					affiche_element(nom,valeur,1);
					uniteCourante=yylex();
					if (uniteCourante==POINT_VIRGULE)
					{
						nom_token(uniteCourante,nom,valeur);
						affiche_element(nom,valeur,1);
						uniteCourante=yylex();
						$$ = cree_n_instr_ecrire($1);
						affiche_balise_fermante(__FUNCTION__,1);
						return $$;
					}
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_instr instructionVide(void)
{
	n_instr *$$ = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==POINT_VIRGULE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_exp expression(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_conjonction_,uniteCourante))
	{
		$1 = conjonction();
		$2 = expressionBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void expressionBis(n_exp h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==OU)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_conjonction_,uniteCourante))
		{
			$1 = conjonction();
			$2 = expressionBis($1);
			$$ = cree_n_exp_op(ou,h,$2);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else if(est_suivant(_expressionBis_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_exp conjonction(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_comparaison_,uniteCourante))
	{
		$1 = comparaison();
		$2 = conjonctionBis($1);
		affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void conjonctionBis(n_exp h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==ET)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_comparaison_,uniteCourante))
		{
			$1 = comparaison();
			$2 = conjonctionBis($1);
			$$ = cree_n_exp_op(et,h,$2);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else if(est_suivant(_conjonctionBis_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void comparaison(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_expArith_,uniteCourante))
	{
		$1 = expArith();
		$2 = comparaisonBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void comparaisonBis(n_exp h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==EGAL) 
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_expArith_,uniteCourante))
		{
			$1 = expArith();
			$2 = comparaisonBis($1);
			$$ = cree_n_exp_op(egal,h,$2);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else if(uniteCourante==INFERIEUR)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_expArith_,uniteCourante))
			{
				$1 = expArith();
				$2 = comparaisonBis($1);
				$$ = cree_n_exp_op(inf,h,$2);
				affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
		}
		else if(est_suivant(_comparaisonBis_,uniteCourante))
			{
				affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void expArith(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (est_premier(_terme_,uniteCourante))
	{
		$1 = terme();
		$2 = expArithBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void expArithBis(n_exp h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==PLUS)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_terme_,uniteCourante))
		{
			$1 = terme();
			$2 = expArithBis($1);
			$$ = cree_n_exp_op(plus,h,$2);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else if(uniteCourante==MOINS) 
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if (est_premier(_terme_,uniteCourante))
			{
				$1 = terme();
				$2 = expArithBis($1);
				$$ = cree_n_exp_op(moins,h,$2);
				affiche_balise_fermante(__FUNCTION__,1);
				return $$;
			}
		}
		else if(est_suivant(_expArithBis_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void terme(void)
{
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_negation_,uniteCourante))
	{
		$1 = negation();
		$2 = termeBis();
		affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void termeBis(n_exp h)
{
	n_exp *$$ = NULL;
	n_exp *$1 = NULL;
	n_exp *$2 = NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if (uniteCourante==FOIS)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			$1 = negation();
			$2 = termeBis($1);
			$$ = cree_n_exp_op(fois,h,$2);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else if(uniteCourante==DIVISE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			$1 = negation();
			$2 = termeBis($1);
			$$ = cree_n_exp_op(divise,h,$2);
			affiche_balise_fermante(__FUNCTION__,1);
			return $$;
		}
	}
	else if(est_suivant(_termeBis_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return $$;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void negation(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==NON)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if (est_premier(_negation_,uniteCourante))
		{
			negation();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else if(est_premier(_facteur_,uniteCourante))
	{
		facteur();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void facteur(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==PARENTHESE_OUVRANTE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			expression();
			if(uniteCourante==PARENTHESE_FERMANTE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
	else if(uniteCourante==NOMBRE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_appelFct_,uniteCourante))
	{
		appelFct();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(est_premier(_var_,uniteCourante))
	{
		var();
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else if(uniteCourante==LIRE) 
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(uniteCourante==PARENTHESE_OUVRANTE) 
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(uniteCourante==PARENTHESE_FERMANTE) 
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void var(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==ID_VAR)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_optIndice_,uniteCourante) || est_suivant(_var_,uniteCourante))
		{
			optIndice();
			affiche_balise_fermante(__FUNCTION__,1);
			return;
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

void optIndice(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==CROCHET_OUVRANT)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			expression();
			if(uniteCourante==CROCHET_FERMANT)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				affiche_balise_fermante(__FUNCTION__,1);
				return;
			}
		}
	}
		
	if(est_suivant(_optIndice_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_appel appelFct(void)
{
	n_appel *$$ = NULL;
	n_l_exp *$1=NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==ID_FCT)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		$1=valeur;
		uniteCourante=yylex();
		if(uniteCourante==PARENTHESE_OUVRANTE)
		{
			nom_token(uniteCourante,nom,valeur);
			affiche_element(nom,valeur,1);
			uniteCourante=yylex();
			if(est_premier(_listeExpressions_,uniteCourante) || est_suivant(_listeExpressions_,uniteCourante))
			{
				$1=listeExpressions();
				if(uniteCourante==PARENTHESE_FERMANTE)
				{
					nom_token(uniteCourante,nom,valeur);
					affiche_element(nom,valeur,1);
					uniteCourante=yylex();
					$$=cree_n_appel(valeur,$1);
					affiche_balise_fermante(__FUNCTION__,1);
					return $$;
				}
			}
			
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_exp listeExpressions()
{
	n_l_exp $$=NULL;
	n_exp *$1=NULL;
	n_l_exp *$2=NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(est_premier(_expression_,uniteCourante))
	{
		$1=expression();
		$2=listeExpressionsBis($1);
		affiche_balise_fermante(__FUNCTION__,1);
		$$=cree_n_l_exp($1,$2);
		return $$;
	}
	else if(est_suivant(_listeExpressions_,uniteCourante))
	{
		affiche_balise_fermante(__FUNCTION__,1);
		return $2;
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

n_l_exp listeExpressionsBis(n_exp h)
{
	n_exp *$1=NULL;
	n_l_exp *$2=NULL;
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==VIRGULE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_expression_,uniteCourante))
		{
			$1=expression();
			$2=listeExpressionsBis($1);
			affiche_balise_fermante(__FUNCTION__,1);
			return $2;
		}
	}
	else if(est_suivant(_listeExpressionsBis_,uniteCourante))
		{
			affiche_balise_fermante(__FUNCTION__,1);
			return $2;
		}
	else erreur_syntaxe();	
	affiche_balise_fermante(__FUNCTION__,1);
}

void instructionFaire(void)
{
	affiche_balise_ouvrante(__FUNCTION__,1);
	if(uniteCourante==FAIRE)
	{
		nom_token(uniteCourante,nom,valeur);
		affiche_element(nom,valeur,1);
		uniteCourante=yylex();
		if(est_premier(_instructionBloc_,uniteCourante))
		{
			instructionBloc();
			if(uniteCourante==TANTQUE)
			{
				nom_token(uniteCourante,nom,valeur);
				affiche_element(nom,valeur,1);
				uniteCourante=yylex();
				if(est_premier(_expression_,uniteCourante))
				{
					expression();
					if(uniteCourante==POINT_VIRGULE)
					{
						nom_token(uniteCourante,nom,valeur);
						affiche_element(nom,valeur,1);
						uniteCourante=yylex();
						affiche_balise_fermante(__FUNCTION__,1);
						return;
					}
				}
			}
		}
	}
	else erreur_syntaxe();
	affiche_balise_fermante(__FUNCTION__,1);
}

int main(int argc, char **argv)
{
	int i;
	for(i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-l")==0)
		{
			analyse_lex = 1;
			continue;
		}
		if(strcmp(argv[i],"-s")==0)
		{
			analyse_synt=1;
			continue;
		}
		yyin = fopen(argv[i], "r");
		if(yyin == NULL){
		fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[1]);
		exit(1);
  }
	}
	if(analyse_lex==1)
	{
		test_yylex_internal(yyin);
	}
	
	if(analyse_synt==1)
	{
		initialise_premiers();
		initialise_suivants();
		uniteCourante = yylex();
		programme();
	}
 
  
  //~ if(uniteCourante == FIN)
  //~ {
	  //~ printf("Expression correcte !\n");
	  //~ return 0;
  //~ }
  //~ else
  //~ {
	  //~ printf("Expression incorrecte !\n");
	  //~ exit(-1);
  //~ }
	return 0;
}
