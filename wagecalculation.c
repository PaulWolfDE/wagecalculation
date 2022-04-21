#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Prozentsätze einzelner Sozialversicherungen
#define KRANKENVERSICHERUNG         14.6    // Krankenversicherung
#define RENTENVERSICHERUNG          18.6    // Rentenversicherung
#define ARBEITSLOSENVERSICHERUNG    2.4     // Arbeitslosenversicherung
#define PFLEGEVERSICHERUNG          3.05    // Pflegeversicherung
#define PFLEGEZUSCHLAG              0.35    // Kinderlose > 23

// Beitragsbemessungsgrenzen einzelner Sozialversicherungen 
#define BEITRAGSBEMESSUNGSGRENZE_WESTEN_RENTE_ARBEITSLOS  7050    // Beitragsbemessungsgrenze Renten- & Arbeitslosenversicherung im Westen
#define BEITRAGSBEMESSUNGSGRENZE_OSTEN_RENTE_ARBEITSLOS   6720    // Beitragsbemessungsgrenze Renten- & Arbeitslosenversicherung im Osten
#define BEITRAGSBEMESSUNGSGRENZE_KRANKEN_PFLEGE           4837.5  // Beitragsbemessungsgrenze Kranken- & Pflegeversicherung

// Kindergeldsentlastungen je Kind
#define KINDERGELD_1_2   209     // Kindergeld für das erste und zweite Kind
#define KINDERGELD_3     215     // Kindergeld für das dritte Kind
#define KINDERGELD_4     240     // Kindergeld ab dem vierten Kind

bool equalsIgnoreCase(const char[], const char[]);

double krankenkassenzusatzbeitrag;
bool keineKinderMit23;
bool osten;
double bruttoeinkommen;
double einkommensteuer;
double steuerbetragPflegeArbeitslos, steuerbetragKrankenPflege;
double result;
int anzahlDerKinder;

double krankenversicherung;
double krankenversicherungszusatz;
double rentenversicherung;
double arbeitlosenversicherung;
double pflegeversicherung;

int main(void)
{
    char str[256];
    
    printf("Wie hoch ist Ihr Bruttoeinkommen?\n");
    scanf("%lf", &bruttoeinkommen);
    
    printf("Wie hoch ist die Lohnsteuer inkl. Kirchensteuer und Solidaritaetszuschlag?\n");
    scanf("%lf", &einkommensteuer);
    
    printf("Wie gross ist Ihr Krankenkassenzusatzbeitragssatz in Prozent?\n");
    scanf("%lf", &krankenkassenzusatzbeitrag);

kindergeldsabfrage:

    printf("Sind Sie ueber 23 und haben keine Kinder? [j/n]\n");
    scanf("%s", str);
    if (equalsIgnoreCase(str, "j") || equalsIgnoreCase(str, "ja"))
        keineKinderMit23 = true;
    else if (equalsIgnoreCase(str, "n") || equalsIgnoreCase(str, "nein"))
        keineKinderMit23 = false;
    else 
        goto kindergeldsabfrage;

ostenOderWestenAbfrage:

    printf("Wohnen Sie im Osten? [j/n]\n");
    scanf("%s", str);
    if (equalsIgnoreCase(str, "j") || equalsIgnoreCase(str, "ja"))
        osten = true;
    else if (equalsIgnoreCase(str, "n") || equalsIgnoreCase(str, "nein"))
        osten = false;
    else 
        goto ostenOderWestenAbfrage;

    if (!keineKinderMit23)
    {
        printf("Wie viele Kinder haben Sie?\n");
        scanf("%d", &anzahlDerKinder);
    }
    else
        anzahlDerKinder = 0;

    if ((osten && bruttoeinkommen > BEITRAGSBEMESSUNGSGRENZE_OSTEN_RENTE_ARBEITSLOS) || (!osten && bruttoeinkommen > BEITRAGSBEMESSUNGSGRENZE_WESTEN_RENTE_ARBEITSLOS))
        steuerbetragPflegeArbeitslos = osten ? BEITRAGSBEMESSUNGSGRENZE_OSTEN_RENTE_ARBEITSLOS : BEITRAGSBEMESSUNGSGRENZE_WESTEN_RENTE_ARBEITSLOS;
    else
        steuerbetragPflegeArbeitslos = bruttoeinkommen;
    if (bruttoeinkommen > BEITRAGSBEMESSUNGSGRENZE_KRANKEN_PFLEGE)
        steuerbetragKrankenPflege = BEITRAGSBEMESSUNGSGRENZE_KRANKEN_PFLEGE;
    else
        steuerbetragKrankenPflege = bruttoeinkommen;
    result = bruttoeinkommen;
    
    printf("\nFuer den Arbeitnehmer:\n");
    printf("---------------------\n");
    printf("Einkommen/Abgabe\t\t| Betrag\n");
    printf("------------------------------------------\n");
    printf("Bruttoeinkommen\t\t\t| +%.2lf\n", bruttoeinkommen);
    printf("Krankenversicherung\t\t| -%.2lf\n", krankenversicherung = steuerbetragKrankenPflege * KRANKENVERSICHERUNG / 2 / 100);
    result -= krankenversicherung;
    printf("Krankenkassenzusatzbeitrag\t| -%.2lf\n", krankenversicherungszusatz = steuerbetragKrankenPflege * krankenkassenzusatzbeitrag / 2 / 100);
    result -= krankenversicherungszusatz;
    printf("Rentenversicherung\t\t| -%.2lf\n", rentenversicherung = steuerbetragPflegeArbeitslos * RENTENVERSICHERUNG / 2 / 100);
    result -= rentenversicherung;
    printf("Arbeitslosenversicherung\t| -%.2lf\n", arbeitlosenversicherung = steuerbetragPflegeArbeitslos * ARBEITSLOSENVERSICHERUNG / 2 / 100);
    result -= arbeitlosenversicherung;
    if (keineKinderMit23)
    {
        printf("Pflegeversicherung\t\t| -%.2lf\n", steuerbetragKrankenPflege * (PFLEGEVERSICHERUNG / 2 + PFLEGEZUSCHLAG) / 100);
        result -= steuerbetragKrankenPflege * (PFLEGEVERSICHERUNG / 2 + PFLEGEZUSCHLAG) / 100;
    }
    else
    {
        printf("Pflegeversicherung\t\t| -%.2lf\n", steuerbetragKrankenPflege * PFLEGEVERSICHERUNG / 2 / 100);
        result -= steuerbetragKrankenPflege * PFLEGEVERSICHERUNG / 2 / 100;
    }
    printf("Lohnsteuer\t\t\t| -%.2lf\n", einkommensteuer);
    result -= einkommensteuer;

    double childBenefit = 0;
    for (int i = 0; i < anzahlDerKinder; i++)
        childBenefit += i > 3 ? KINDERGELD_4 : i > 2 ? KINDERGELD_3 : KINDERGELD_1_2;

    printf("Kindergeldsentlastung\t\t| +%.2lf\n", childBenefit);
    result += childBenefit;

    char buffer[256]; 
    sprintf(buffer, "Netto: %.2lf", result);
    printf("\n%s\n", buffer);
    for (int i = 0; i < strlen(buffer); i++)
        printf("-");
    printf("\n\n");
    
    printf("Fuer den Arbeitgeber:\n");
    printf("---------------------\n");
    printf("Versicherung\t\t\t| Abgabe\n");
    printf("------------------------------------------\n");
    printf("Krankenversicherung\t\t| %.2lf\n", krankenversicherung);
    printf("Krankenkassenzusatzbeitrag\t| %.2lf\n", krankenversicherungszusatz);
    printf("Rentenversicherung\t\t| %.2lf\n", rentenversicherung);
    printf("Arbeitslosenversicherung\t| %.2lf\n", arbeitlosenversicherung);
    printf("Pflegeversicherung\t\t| %.2lf\n", pflegeversicherung = steuerbetragKrankenPflege * PFLEGEVERSICHERUNG / 2 / 100);
    buffer = sprintf("Kosten fuer den Arbeitgeber: %.2lf", krankenversicherung + krankenversicherungszusatz + rentenversicherung + arbeitlosenversicherung + pflegeversicherung);
    printf("\n%s\n", buffer);
    for (int i = 0; i < strlen(buffer); i++)
        printf("-");
    printf("\n\n");
    
    return 0;
}

bool equalsIgnoreCase(const char a[], const char b[])
{
    if (strlen(a) != strlen(b))
        return false;
    for (int i = 0; i < strlen(a); i++)
        if (toupper(a[i]) != toupper(b[i]))
            return false;
    return true;
}
