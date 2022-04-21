import java.util.Scanner;

class Main {

	private static final double HEALTH_INSURANCE		= 14.6;
	private static final double PENSION_INSURANCE		= 18.6;
	private static final double UNEMPLOYED_INSURANCE	= 2.4;
	private static final double NURSING_INSURANCE		= 3.05;
	private static final double NURSING_SURCHARGE		= 0.35;

	private static final double ASSESMENT_CEILING_WEST_PENSION_UNEMPLOYED 	= 7050;
	private static final double ASSESMENT_CEILING_EAST_PENSION_UNEMPLOYED 	= 6720;
	private static final double ASSESMENT_CEILING_HEALTH_NURSING          	= 4837.5;

	private static final double CHILD_BENEFIT_1_2	= 209;
	private static final double CHILD_BENEFIT_3		= 215;
	private static final double CHILD_BENEFIT_4		= 240;

	private static double healthInsuranceContribution; 
	private static double income;
	private static double payrollTax;
	private static double taxAmountPU, taxAmountHN;
	private static double result;
	private static int numberOfChildren;
	private static boolean noKidsAt23;
	private static boolean east;

	private static double healthInsurance;
	private static double healthInsuranceAdditive;
	private static double pensionInsurance;
	private static double unemployedInsurance;
	private static double nursingInsurance;

	public static void main(String[] args) {

		String input;
		Scanner scanner = new Scanner(System.in);

		System.out.printf("Wie hoch ist Ihr Bruttoeinkommen im Monat?\n");
		income = scanner.nextDouble();
		System.out.printf("Wie hoch ist die Lohnsteuer inkl. Kirchensteuer und Solidaritaetszuschlag?\n");
		payrollTax = scanner.nextDouble();
		System.out.printf("Wie gross ist Ihr Krankenkassenzusatzbeitragssatz in Prozent?\n");
		healthInsuranceContribution = scanner.nextDouble();

		promptChildren();

		promptEast();

		if (!noKidsAt23) {
			System.out.printf("Wie viele Kinder haben Sie?\n");
			numberOfChildren = scanner.nextInt();
		}
		else
			numberOfChildren = 0;

		if ((east && income > ASSESMENT_CEILING_EAST_PENSION_UNEMPLOYED) || (!east && income > ASSESMENT_CEILING_WEST_PENSION_UNEMPLOYED))
			taxAmountPU = east ? ASSESMENT_CEILING_EAST_PENSION_UNEMPLOYED : ASSESMENT_CEILING_WEST_PENSION_UNEMPLOYED;
		else
			taxAmountPU = income;
		if (income > ASSESMENT_CEILING_HEALTH_NURSING)
			taxAmountHN = ASSESMENT_CEILING_HEALTH_NURSING;
		else
			taxAmountHN = income;

		result = income;

		System.out.printf("\nFuer den Arbeitnehmer:\n");
		System.out.printf("---------------------\n");
		System.out.printf("Einkommen/Abgabe\t\t| Betrag\n");
		System.out.printf("------------------------------------------\n");
		System.out.printf("Bruttoeinkommen\t\t\t| +%.2f\n", income);
		System.out.printf("Krankenversicherung\t\t| -%.2f\n", healthInsurance = taxAmountHN * HEALTH_INSURANCE / 2 / 100);
		result -= healthInsurance;
		System.out.printf("Krankenkassenzusatzbeitragssatz\t| -%.2f\n", healthInsuranceAdditive = taxAmountHN * healthInsuranceContribution / 2 / 100);
		result -= healthInsuranceAdditive;
		System.out.printf("Rentenversicherung\t\t| -%.2f\n", pensionInsurance = taxAmountPU * PENSION_INSURANCE / 2 / 100);
		result -= pensionInsurance;
		System.out.printf("Arbeitslosenversicherung\t| -%.2f\n", unemployedInsurance = taxAmountPU * UNEMPLOYED_INSURANCE / 2 / 100);
		result -= unemployedInsurance;
		if (noKidsAt23)	{
			System.out.printf("Pflegeversicherung\t\t| -%.2f\n", taxAmountHN * (NURSING_INSURANCE / 2 + NURSING_SURCHARGE) / 100);
			result -= taxAmountHN * (NURSING_INSURANCE / 2 + NURSING_SURCHARGE) / 100;
		} else {
			System.out.printf("Pflegeversicherung\t\t| -%.2f\n", taxAmountHN * NURSING_INSURANCE / 2 / 100);
			result -= taxAmountHN * NURSING_INSURANCE / 2 / 100;
		}
		System.out.printf("Lohnsteuer\t\t\t| -%.2f\n", payrollTax);
		result -= payrollTax;

		double childBenefit = 0;
		for (int i = 0; i < numberOfChildren; i++)
			childBenefit += i > 3 ? CHILD_BENEFIT_4 : i > 2 ? CHILD_BENEFIT_3 : CHILD_BENEFIT_1_2;

		System.out.printf("Kindergeldsentlastung\t\t| +%.2f\n", childBenefit);
		result += childBenefit;

		String buffer = String.format("Netto: %.2f", result);
		System.out.printf("\n%s\n", buffer);
		for (int i = 0; i < buffer.length(); i++)
			System.out.printf("-");
		System.out.printf("\n\n");

		System.out.printf("Fuer den Arbeitgeber:\n");
		System.out.printf("--------------------\n");
		System.out.printf("Versicherung\t\t\t| Abgabe\n");
		System.out.printf("------------------------------------------\n");
		System.out.printf("Krankenversicherung\t\t| %.2f\n", healthInsurance);
		System.out.printf("Krankenkassenzusatzbeitrag\t| %.2f\n", healthInsuranceAdditive);
		System.out.printf("Rentenversicherung\t\t| %.2f\n", pensionInsurance);
		System.out.printf("Arbeitslosenversicherung\t| %.2f\n", unemployedInsurance);
		System.out.printf("Pflegeversicherung\t\t| %.2f\n",nursingInsurance = taxAmountHN * NURSING_INSURANCE / 2 / 100);
		buffer = String.format("Kosten fuer den Arbeitgeber: %.2f", healthInsurance + healthInsuranceAdditive + pensionInsurance + unemployedInsurance + nursingInsurance);
		System.out.printf("\n%s\n", buffer);
		for (int i = 0; i < buffer.length(); i++)
			System.out.printf("-");
		System.out.printf("\n\n");
	}

	private static void promptChildren() {

		String str;
		Scanner scanner = new Scanner(System.in);

		System.out.printf("Sind Sie ueber 23 und haben keine Kinder? [j/n]\n");
		str = scanner.next();
		if (str.equalsIgnoreCase("j") || str.equalsIgnoreCase("ja"))
			noKidsAt23 = true;
		else if (str.equalsIgnoreCase("n") || str.equalsIgnoreCase("nein"))
			noKidsAt23 = false;
		else 
			promptChildren();
	}

	private static void promptEast() {

		String str;
		Scanner scanner = new Scanner(System.in);

		System.out.printf("Wohnen Sie im Osten? [j/n]\n");
		str = scanner.next();
		if (str.equalsIgnoreCase("j") || str.equalsIgnoreCase("ja"))
			east = true;
		else if (str.equalsIgnoreCase("n") || str.equalsIgnoreCase("nein"))
			east = false;
		else 
			promptEast();
	}
}