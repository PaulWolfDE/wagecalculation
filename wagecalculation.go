package main

import (
	"fmt"
	"strings"
)

const (
	HEALTH_INSURANCE		= 14.6
	PENSION_INSURANCE		= 18.6
	UNEMPLOYED_INSURANCE	= 2.4
	NURSING_INSURANCE		= 3.05
	NURSING_SURCHARGE		= 0.35

	ASSESMENT_CEILING_WEST_PENSION_UNEMPLOYED = 7050
	ASSESMENT_CEILING_EAST_PENSION_UNEMPLOYED = 6720
	ASSESMENT_CEILING_HEALTH_NURSING          = 4837.5

	CHILD_BENEFIT_1_2 = 209
	CHILD_BENEFIT_3   = 215
	CHILD_BENEFIT_4   = 240
)

var (
	healthInsuranceContribution float64
	noKidsAt23                  bool
	east                        bool
	income                      float64
	payrollTax                  float64
	taxAmountPU, taxAmountHN    float64
	result                      float64
	numberOfChildren            int
)

var (
	healthInsurance     	float64
	healthInsuranceAdditive float64
	pensionInsurance    	float64
	unemployedInsurance 	float64
	nursingInsurance    	float64
)

func main() {

	var input string

	fmt.Printf("Wie hoch ist Ihr Bruttoeinkommen im Monat?\n")
	fmt.Scanf("%f", &income)

	fmt.Printf("Wie hoch ist die Lohnsteuer inkl. Kirchensteuer und Solidaritaetszuschlag?\n")
	fmt.Scanf("%f", &payrollTax)

	fmt.Printf("Wie gross ist Ihr Krankenkassenzusatzbeitragssatz in Prozent?\n")
	fmt.Scanf("%f", &healthInsuranceContribution)

children:
	
	fmt.Printf("Sind Sie ueber 23 und haben keine Kinder? [j/n]\n")
	fmt.Scanf("%s", &input)
	if strings.EqualFold(input, "j") || strings.EqualFold(input, "ja") {
		noKidsAt23 = true
	} else if strings.EqualFold(input, "n") || strings.EqualFold(input, "nein") {
		noKidsAt23 = false
	} else {
		goto children
	}

eastOrWest:

	fmt.Printf("Wohnen Sie im Osten? [j/n]\n")
	fmt.Scanf("%s", &input)
	if strings.EqualFold(input, "j") || strings.EqualFold(input, "ja") {
		east = true
	} else if strings.EqualFold(input, "n") || strings.EqualFold(input, "nein") {
		east = false
	} else {
		goto eastOrWest
	}

	if !noKidsAt23 {
		fmt.Printf("Wie viele Kinder haben Sie?\n")
		fmt.Scanf("%d", &numberOfChildren)
	} else {
		numberOfChildren = 0
	}

	if (east && income > ASSESMENT_CEILING_EAST_PENSION_UNEMPLOYED) || (!east && income > ASSESMENT_CEILING_WEST_PENSION_UNEMPLOYED) {
		if east {
			taxAmountPU = ASSESMENT_CEILING_EAST_PENSION_UNEMPLOYED
		} else {
			taxAmountPU = ASSESMENT_CEILING_WEST_PENSION_UNEMPLOYED
		}
	} else {
		taxAmountPU = income
	}
	if income > ASSESMENT_CEILING_HEALTH_NURSING {
		taxAmountHN = ASSESMENT_CEILING_HEALTH_NURSING
	} else {
		taxAmountHN = income
	}
	result = income

	fmt.Printf("\nFuer den Arbeitnehmer:\n");
    fmt.Printf("---------------------\n");
    fmt.Printf("Einkommen/Abgabe\t\t| Betrag\n");
    fmt.Printf("------------------------------------------\n");
    fmt.Printf("Bruttoeinkommen\t\t\t| +%.2f\n", income);
    healthInsurance = taxAmountHN * HEALTH_INSURANCE / 2 / 100
    fmt.Printf("Krankenversicherung\t\t| -%.2f\n", healthInsurance);
    result -= healthInsurance;
    healthInsuranceAdditive = taxAmountHN * healthInsuranceContribution / 2 / 100
    fmt.Printf("Krankenkassenzusatzbeitrag\t| -%.2f\n", healthInsuranceAdditive);
    result -= healthInsuranceAdditive;
    pensionInsurance = taxAmountPU * PENSION_INSURANCE / 2 / 100
    fmt.Printf("Rentenversicherung\t\t| -%.2f\n", pensionInsurance);
    result -= pensionInsurance;
    unemployedInsurance = taxAmountPU * UNEMPLOYED_INSURANCE / 2 / 100
    fmt.Printf("Arbeitslosenversicherung\t| -%.2f\n", unemployedInsurance);
    result -= unemployedInsurance;
    if noKidsAt23 {
    	fmt.Printf("Pflegeversicherung\t\t| -%.2f\n", taxAmountHN * (NURSING_INSURANCE / 2 + NURSING_SURCHARGE) / 100)
    	result -= taxAmountHN * (NURSING_INSURANCE / 2 + NURSING_SURCHARGE) / 100
    } else {
    	fmt.Printf("Pflegeversicherung\t\t| -%.2f\n", taxAmountHN *NURSING_INSURANCE / 2 / 100)
    	result -= taxAmountHN * NURSING_INSURANCE / 2 / 100
    }
    fmt.Printf("Lohnsteuer\t\t\t| -%.2f\n", payrollTax)
	result -= payrollTax

	var childBenefit float64 = 0
	for i := 0; i < numberOfChildren; i++ {
		if i > 3 {
			childBenefit += CHILD_BENEFIT_4
		} else if i > 2 {
			childBenefit += CHILD_BENEFIT_3
		} else {
			childBenefit += CHILD_BENEFIT_1_2
		}
	}

	fmt.Printf("Kindergeldsentlastung\t\t| +%.2f\n", childBenefit)
	result += childBenefit

	var buffer string
	buffer = fmt.Sprintf("Netto: %.2f", result)
	fmt.Printf("\n%s\n", buffer)
	for i := 0; i < len(buffer); i++ {
		fmt.Printf("-")
	}

	fmt.Printf("\n\n")

	fmt.Printf("Fuer den Arbeitgeber:\n");
    fmt.Printf("---------------------\n");
    fmt.Printf("Versicherung\t\t\t| Abgabe\n");
    fmt.Printf("------------------------------------------\n");
    fmt.Printf("Krankenversicherung\t\t| %.2f\n", healthInsurance);
    fmt.Printf("Krankenkassenzusatzbeitrag\t| %.2f\n", healthInsuranceAdditive);
    fmt.Printf("Rentenversicherung\t\t| %.2f\n", pensionInsurance);
    fmt.Printf("Arbeitslosenversicherung\t| %.2f\n", unemployedInsurance);
    nursingInsurance = taxAmountHN * NURSING_INSURANCE / 2 / 100
    fmt.Printf("Pflegeversicherung\t\t| %.2f\n", nursingInsurance)
    buffer =fmt.Sprintf("Kosten fuer den Arbeitgeber: %.2f", healthInsurance + healthInsuranceAdditive + pensionInsurance + unemployedInsurance + nursingInsurance);
    fmt.Printf("\n%s\n", buffer)
	for i := 0; i < len(buffer); i++ {
		fmt.Printf("-")
	}
	fmt.Printf("\n\n")
}