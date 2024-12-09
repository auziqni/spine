
#include <Arduino.h>
// Konstanta untuk toleransi perbandingan float
const float EPSILON = 0.0001;

// Struct untuk menyimpan hasil perbandingan
struct ComparisonResult
{
    int index;
    float value;
    bool isValid;
    String message;
};

ComparisonResult compareValues(float nilai_normalis, float nilai_lordosis,
                               float nilai_kifosis, float nilai_skoliosis)
{
    ComparisonResult result = {0, 0.0, true, ""};

    // Validasi input - cek untuk NaN atau Infinity
    if (isnan(nilai_normalis) || isnan(nilai_lordosis) ||
        isnan(nilai_kifosis) || isnan(nilai_skoliosis) ||
        isinf(nilai_normalis) || isinf(nilai_lordosis) ||
        isinf(nilai_kifosis) || isinf(nilai_skoliosis))
    {
        result.isValid = false;
        result.message = "Error: Input tidak valid (NaN atau Infinity)";
        return result;
    }

    // Validasi range (contoh: nilai harus positif dan kurang dari 360 derajat)
    if (nilai_normalis < 0 || nilai_lordosis < 0 ||
        nilai_kifosis < 0 || nilai_skoliosis < 0 ||
        nilai_normalis > 360 || nilai_lordosis > 360 ||
        nilai_kifosis > 360 || nilai_skoliosis > 360)
    {
        result.isValid = false;
        result.message = "Error: Nilai harus antara 0 dan 360 derajat";
        return result;
    }

    // Array untuk menyimpan semua nilai dan indeksnya
    struct ValuePair
    {
        float value;
        int index;
        String name;
    };

    ValuePair values[] = {
        {nilai_normalis, 0, "Normalis"},
        {nilai_lordosis, 1, "Lordosis"},
        {nilai_kifosis, 2, "Kifosis"},
        {nilai_skoliosis, 3, "Skoliosis"}};

    // Debug print semua input
    Serial.println("\n=== Input Values ===");
    for (int i = 0; i < 4; i++)
    {
        Serial.print(values[i].name);
        Serial.print(": ");
        Serial.println(values[i].value, 6); // 6 digit presisi
    }

    // Cari nilai maksimum
    ValuePair maxPair = values[0];
    int countEqual = 0; // Hitung berapa nilai yang sama dengan maksimum

    for (int i = 0; i < 4; i++)
    {
        if (abs(values[i].value - maxPair.value) <= EPSILON)
        {
            countEqual++;
        }
        else if (values[i].value > maxPair.value)
        {
            maxPair = values[i];
            countEqual = 1;
        }
    }

    // Jika ada beberapa nilai yang sama
    if (countEqual > 1)
    {
        Serial.println("\n=== WARNING: Multiple Equal Values Detected ===");
        Serial.print("Found ");
        Serial.print(countEqual);
        Serial.println(" equal maximum values:");

        for (int i = 0; i < 4; i++)
        {
            if (abs(values[i].value - maxPair.value) <= EPSILON)
            {
                Serial.print("- ");
                Serial.print(values[i].name);
                Serial.print(": ");
                Serial.println(values[i].value, 6);
            }
        }
    }

    // Set hasil
    result.index = maxPair.index;
    result.value = maxPair.value;
    result.message = "Nilai tertinggi: " + maxPair.name;

    // Debug print hasil
    Serial.println("\n=== Result ===");
    Serial.print("Index: ");
    Serial.println(result.index);
    Serial.print("Value: ");
    Serial.println(result.value, 6);
    Serial.print("Message: ");
    Serial.println(result.message);
    Serial.println("==================\n");

    return result;
}

// Fungsi helper untuk debug
void printComparisonResult(ComparisonResult result)
{
    if (!result.isValid)
    {
        Serial.println("INVALID RESULT!");
        Serial.println(result.message);
        return;
    }

    Serial.println("\nFinal Result:");
    Serial.print("Index: ");
    Serial.println(result.index);
    Serial.print("Value: ");
    Serial.println(result.value, 6);
    Serial.print("Message: ");
    Serial.println(result.message);
}

// Contoh penggunaan:
void setup()
{
    Serial.begin(9600);

    // Test case 1: Normal case
    ComparisonResult result1 = compareValues(1.5, 2.7, 1.2, 0.8);
    printComparisonResult(result1);

    // Test case 2: Equal values
    ComparisonResult result2 = compareValues(2.0, 2.0, 1.5, 1.8);
    printComparisonResult(result2);

    // Test case 3: Invalid input
    ComparisonResult result3 = compareValues(-1.0, 2.0, 1.5, 1.8);
    printComparisonResult(result3);
}