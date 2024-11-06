#include "Arduino.h"
#include "Wire.h"
#include <Fuzzy.h>
// #include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x27, 20, 4);
unsigned long timer = 0;
float sumbux1, sumbux2, sumbux3, sumbux4, sumbux5, sumbux6;
float sumbuy1, sumbuy2, sumbuy3, sumbuy4, sumbuy5, sumbuy6;
float sumbuz1, sumbuz2, sumbuz3, sumbuz4, sumbuz5, sumbuz6;
float linex;
Fuzzy *fuzzy = new Fuzzy();

// FuzzyInput1 ccx
FuzzySet *normal1 = new FuzzySet(73, 82, 90, 90);
FuzzySet *kelainan1 = new FuzzySet(50, 50, 73, 82);

// FuzzyInput2 tcx
FuzzySet *normal2 = new FuzzySet(63, 74.5, 90, 90);
FuzzySet *kelainan2 = new FuzzySet(40, 40, 63, 74.5);

// FuzzyInput3 tcz
FuzzySet *normal3 = new FuzzySet(-17, -5.5, -5.5, 17);
FuzzySet *kelainankanan3 = new FuzzySet(5.5, 17, 40, 40);
FuzzySet *kelainankiri3 = new FuzzySet(-40, -40, -17, -5.5);

// FuzzyInput4 lcx
FuzzySet *normal4 = new FuzzySet(73, 82, 90, 90);
FuzzySet *kelainan4 = new FuzzySet(50, 50, 73, 82);

// FuzzyInput5 lcz
FuzzySet *normal5 = new FuzzySet(-17, -5.5, -5.5, 17);
FuzzySet *kelainankanan5 = new FuzzySet(5.5, 17, 40, 40);
FuzzySet *kelainankiri5 = new FuzzySet(-40, -40, -17, -5.5);

// FuzzyInput6 tcx
FuzzySet *normal6 = new FuzzySet(63, 74.5, 90, 90);
FuzzySet *kelainan6 = new FuzzySet(40, 40, 63, 74.5);

// FuzzyOutput
FuzzySet *normalis = new FuzzySet(0, 0, 8, 12.75);
FuzzySet *lordosis = new FuzzySet(8, 12.75, 18.25, 22.75);
FuzzySet *kifosis = new FuzzySet(18.25, 22.75, 28.25, 32.75);
FuzzySet *skoliosis = new FuzzySet(28.25, 32.75, 40, 40);

void fuzzyRule()
{
    //===========================================================================================================
    // Building FuzzyRule 1
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5, normal6);

    FuzzyRuleConsequent *thenHasilNormal = new FuzzyRuleConsequent();
    thenHasilNormal->addOutput(normalis);
    FuzzyRule *fuzzyRule1 = new FuzzyRule(1, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilNormal);
    fuzzy->addFuzzyRule(fuzzyRule1);

    // Building FuzzyRule 2
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5, normal6);

    FuzzyRuleConsequent *thenHasilKifosis = new FuzzyRuleConsequent();
    thenHasilKifosis->addOutput(kifosis);
    FuzzyRule *fuzzyRule2 = new FuzzyRule(2, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilKifosis);
    fuzzy->addFuzzyRule(fuzzyRule2);

    // Building FuzzyRule 3
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilKifosis= new FuzzyRuleConsequent();
    thenHasilKifosis->addOutput(kifosis);
    FuzzyRule *fuzzyRule3 = new FuzzyRule(3, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilKifosis);
    fuzzy->addFuzzyRule(fuzzyRule3);

    // Building FuzzyRule 4
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5, normal6);

    FuzzyRuleConsequent *thenHasilSkoliosis = new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule4 = new FuzzyRule(4, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule4);

    // Building FuzzyRule 5
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule5 = new FuzzyRule(5, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule5);

    // Building FuzzyRule 6
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5, normal6);

    FuzzyRuleConsequent *thenHasilLordosis = new FuzzyRuleConsequent();
    thenHasilLordosis->addOutput(lordosis);
    FuzzyRule *fuzzyRule6 = new FuzzyRule(6, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilLordosis);
    fuzzy->addFuzzyRule(fuzzyRule6);

    // Building FuzzyRule 7
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule7 = new FuzzyRule(7, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule7);

    // Building FuzzyRule 8
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule8 = new FuzzyRule(8, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule8);

    // Building FuzzyRule 9
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, normal4);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilLordosis= new FuzzyRuleConsequent();
    thenHasilLordosis->addOutput(lordosis);
    FuzzyRule *fuzzyRule9 = new FuzzyRule(9, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxKelainan6, thenHasilLordosis);
    fuzzy->addFuzzyRule(fuzzyRule9);

    // Building FuzzyRule 10
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, kelainan4);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilLordosis= new FuzzyRuleConsequent();
    thenHasilLordosis->addOutput(lordosis);
    FuzzyRule *fuzzyRule10 = new FuzzyRule(10, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilLordosis);
    fuzzy->addFuzzyRule(fuzzyRule10);

    // Building FuzzyRule 11
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilKifosis= new FuzzyRuleConsequent();
    thenHasilKifosis->addOutput(kifosis);
    FuzzyRule *fuzzyRule11 = new FuzzyRule(11, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilKifosis);
    fuzzy->addFuzzyRule(fuzzyRule11);

    // Building FuzzyRule 12
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule12 = new FuzzyRule(12, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule12);

    // Building FuzzyRule 13
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule13 = new FuzzyRule(13, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule13);

    // Building FuzzyRule 14
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule14 = new FuzzyRule(14, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule14);

    // Building FuzzyRule 15
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule15 = new FuzzyRule(15, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule15);

    // Building FuzzyRule 16
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule16 = new FuzzyRule(16, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule16);

    // Building FuzzyRule 17
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule17 = new FuzzyRule(17, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule17);

    // Building FuzzyRule 18
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule18 = new FuzzyRule(18, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule18);

    // Building FuzzyRule 19
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule19 = new FuzzyRule(19, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule19);

    // Building FuzzyRule 20
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule20 = new FuzzyRule(20, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule20);

    // Building FuzzyRule 21
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule21 = new FuzzyRule(21, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule21);

    // Building FuzzyRule 22
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule22 = new FuzzyRule(22, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule22);

    // Building FuzzyRule 23
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule23 = new FuzzyRule(23, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule23);

    // Building FuzzyRule 24
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule24 = new FuzzyRule(24, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule24);

    // Building FuzzyRule 25
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule25 = new FuzzyRule(25, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule25);

    // Building FuzzyRule 26
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule26 = new FuzzyRule(26, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule26);

    // Building FuzzyRule 27
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule27 = new FuzzyRule(27, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule27);

    // Building FuzzyRule 28
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule28 = new FuzzyRule(28, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule28);

    // Building FuzzyRule 29
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule29 = new FuzzyRule(29, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule29);

    // Building FuzzyRule 30
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule30 = new FuzzyRule(30, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule30);

    // Building FuzzyRule 31
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule31 = new FuzzyRule(31, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule31);

    // Building FuzzyRule 32
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule32 = new FuzzyRule(32, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule32);

    // Building FuzzyRule 33
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule33 = new FuzzyRule(33, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule33);

    // Building FuzzyRule 34
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule34 = new FuzzyRule(34, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule34);

    // Building FuzzyRule 35
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule35 = new FuzzyRule(35, ifccxNormal1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule35);

    // Building FuzzyRule 36
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule36 = new FuzzyRule(36, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule36);

    // Building FuzzyRule 37
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule37 = new FuzzyRule(37, ifccxNormal1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule37);

    // Building FuzzyRule 38
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule38 = new FuzzyRule(38, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule38);

    // Building FuzzyRule 39
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2->joinWithAND(normal1, normal2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxNormal2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6->joinWithAND(ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5, normal6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule39 = new FuzzyRule(39, ifccxNormal1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxNormal6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule39);

    // Building FuzzyRule 40
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule40 = new FuzzyRule(40, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczNormal5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule40);

    // Building FuzzyRule 41
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule41 = new FuzzyRule(41, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczNormal5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule41);

    // Building FuzzyRule 42
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule42 = new FuzzyRule(42, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule42);

    // Building FuzzyRule 43
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule43 = new FuzzyRule(43, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule43);

    // Building FuzzyRule 44
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule44 = new FuzzyRule(44, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule44);

    // Building FuzzyRule 45
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule45 = new FuzzyRule(45, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule45);

    // Building FuzzyRule 46
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule46 = new FuzzyRule(46, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule46);

    // Building FuzzyRule 47
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, normal4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule47 = new FuzzyRule(47, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxNormal4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule47);

    // Building FuzzyRule 48
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule48 = new FuzzyRule(48, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule48);

    // Building FuzzyRule 49
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule49 = new FuzzyRule(49, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule49);

    // Building FuzzyRule 50
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule50 = new FuzzyRule(50, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule50);

    // Building FuzzyRule 51
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxNormal2, normal3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule51 = new FuzzyRule(51, ifccxKelainan1AndtcxNormal2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule51);

    // Building FuzzyRule 52
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule52 = new FuzzyRule(52, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule52);

    // Building FuzzyRule 53
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule53 = new FuzzyRule(53, ifccxKelainan1AndtcxNormal2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule53);

    // Building FuzzyRule 54
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule54 = new FuzzyRule(54, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule54);

    // Building FuzzyRule 55
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2->joinWithAND(kelainan1, normal2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxNormal2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule55 = new FuzzyRule(55, ifccxKelainan1AndtcxNormal2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule55);

    // Building FuzzyRule 56
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule56 = new FuzzyRule(56, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule56);

    // Building FuzzyRule 57
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule57 = new FuzzyRule(57, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule57);

    // Building FuzzyRule 58
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule58 = new FuzzyRule(58, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule58);

    // Building FuzzyRule 59
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxNormal1AndtcxKelainan2, normal3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule59 = new FuzzyRule(59, ifccxNormal1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule59);

    // Building FuzzyRule 60
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule60 = new FuzzyRule(60, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule60);

    // Building FuzzyRule 61
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule61 = new FuzzyRule(61, ifccxNormal1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule61);

    // Building FuzzyRule 62
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule62 = new FuzzyRule(62, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule62);

    // Building FuzzyRule 63
    // FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2->joinWithAND(normal1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxNormal1AndtcxKelainan2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule63 = new FuzzyRule(63, ifccxNormal1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule63);

    // Building FuzzyRule 64
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule64 = new FuzzyRule(64, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule64);

    // Building FuzzyRule 65
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule65 = new FuzzyRule(65, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule65);

    // Building FuzzyRule 66
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule66 = new FuzzyRule(66, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule66);

    // Building FuzzyRule 67
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule67 = new FuzzyRule(67, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule67);

    // Building FuzzyRule 68
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule68 = new FuzzyRule(68, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule68);

    // Building FuzzyRule 69
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankiri3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule69 = new FuzzyRule(69, ifccxKelainan1AndtcxKelainan2AndtczKelainankiri3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule69);

    // Building FuzzyRule 70
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, kelainankiri5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule70 = new FuzzyRule(70, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankiri5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule70);

    // Building FuzzyRule 71
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3->joinWithAND(ifccxKelainan1AndtcxKelainan2, kelainankanan3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4, kelainankanan5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilSkoliosis->addOutput(skoliosis);
    FuzzyRule *fuzzyRule71 = new FuzzyRule(71, ifccxKelainan1AndtcxKelainan2AndtczKelainankanan3AndlcxKelainan4AndlczKelainankanan5AndscxKelainan6, thenHasilSkoliosis);
    fuzzy->addFuzzyRule(fuzzyRule71);

    // Building FuzzyRule 72
    // FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2->joinWithAND(kelainan1, kelainan2);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3->joinWithAND(ifccxKelainan1AndtcxKelainan2, normal3);
    //  FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3, kelainan4);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczNormal5 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczNormal5->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4, normal5);
    FuzzyRuleAntecedent *ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6 = new FuzzyRuleAntecedent();
    ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6->joinWithAND(ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczNormal5, kelainan6);

    // FuzzyRuleConsequent *thenHasilSkoliosis= new FuzzyRuleConsequent();
    thenHasilKifosis->addOutput(kifosis);
    FuzzyRule *fuzzyRule72 = new FuzzyRule(72, ifccxKelainan1AndtcxKelainan2AndtczNormal3AndlcxKelainan4AndlczNormal5AndscxKelainan6, thenHasilKifosis);
    fuzzy->addFuzzyRule(fuzzyRule72);
}

void setup()
{
    Serial.begin(115200);

    // FuzzyInput ccx
    FuzzyInput *ccx = new FuzzyInput(1);
    ccx->addFuzzySet(normal1);
    ccx->addFuzzySet(kelainan1);
    fuzzy->addFuzzyInput(ccx);

    // FuzzyInput tcx
    FuzzyInput *tcx = new FuzzyInput(2);
    tcx->addFuzzySet(normal2);
    tcx->addFuzzySet(kelainan2);
    fuzzy->addFuzzyInput(tcx);

    // FuzzyInput tcz
    FuzzyInput *tcz = new FuzzyInput(3);
    tcz->addFuzzySet(normal3);
    tcz->addFuzzySet(kelainankanan3);
    tcz->addFuzzySet(kelainankiri3);
    fuzzy->addFuzzyInput(tcz);

    // FuzzyInput lcx
    FuzzyInput *lcx = new FuzzyInput(4);
    lcx->addFuzzySet(normal4);
    lcx->addFuzzySet(kelainan4);
    fuzzy->addFuzzyInput(lcx);

    // FuzzyInput lcz
    FuzzyInput *lcz = new FuzzyInput(5);
    lcz->addFuzzySet(normal5);
    lcz->addFuzzySet(kelainankanan5);
    lcz->addFuzzySet(kelainankiri5);
    fuzzy->addFuzzyInput(lcz);

    // FuzzyInput scx
    FuzzyInput *scx = new FuzzyInput(6);
    scx->addFuzzySet(normal6);
    scx->addFuzzySet(kelainan6);
    fuzzy->addFuzzyInput(scx);

    // FuzzyOutput
    FuzzyOutput *hasil = new FuzzyOutput(1);

    hasil->addFuzzySet(normalis);
    hasil->addFuzzySet(lordosis);
    hasil->addFuzzySet(kifosis);
    hasil->addFuzzySet(skoliosis);
    fuzzy->addFuzzyOutput(hasil);

    fuzzyRule();
}

void ambilsen1()
{
    // mpu.update();
    // sumbux1 = mpu.getAngleX();
    sumbux1 = 0;
}

void ambilsen2()
{
    // mpu.update();
    // sumbux2 = mpu.getAngleX();
    // sumbuz2 = mpu.getAngleZ();
    sumbux2 = 0;
    sumbuz2 = 0;
}

void ambilsen3()
{
    // mpu.update();
    // sumbux3 = mpu.getAngleX();
    // sumbuz3 = mpu.getAngleZ();
    sumbux3 = 0;
    sumbuz3 = 0;
}

void ambilsen4()
{
    // mpu.update();
    // sumbux4 = mpu.getAngleX();
    sumbux4 = 0;
}

void loop()
{

    // data sensor1
    ambilsen1();
    delay(200);

    // data sensor2
    ambilsen2();
    delay(200);

    // data sensor3
    ambilsen3();
    delay(200);

    // data sensor4
    ambilsen4();
    delay(200);

    fuzzy->setInput(1, sumbux1);
    fuzzy->setInput(2, sumbux2);
    fuzzy->setInput(3, sumbuz2);
    fuzzy->setInput(4, sumbux3);
    fuzzy->setInput(5, sumbuz3);
    fuzzy->setInput(6, sumbux4);
    fuzzy->fuzzify();

    Serial.print("sumbux1: normal1-> ");
    Serial.print(normal1->getPertinence());
    Serial.print(", kelainan1-> ");
    Serial.println(kelainan1->getPertinence());

    Serial.print("sumbux2: normal2-> ");
    Serial.print(normal2->getPertinence());
    Serial.print(", kelainan2-> ");
    Serial.println(kelainan2->getPertinence());

    Serial.print("sumbuz2: normal3-> ");
    Serial.print(normal3->getPertinence());
    Serial.print(", kelainankiri3-> ");
    Serial.print(kelainankiri3->getPertinence());
    Serial.print(", kelainankanan3-> ");
    Serial.println(kelainankanan3->getPertinence());

    Serial.print("sumbux4: normal4-> ");
    Serial.print(normal4->getPertinence());
    Serial.print(", kelainan4-> ");
    Serial.println(kelainan4->getPertinence());

    Serial.print("sumbuz5: normal5-> ");
    Serial.print(normal5->getPertinence());
    Serial.print(", kelainankiri5-> ");
    Serial.print(kelainankiri5->getPertinence());
    Serial.print(", kelainankanan5-> ");
    Serial.println(kelainankanan5->getPertinence());

    Serial.print("sumbux6: normal6-> ");
    Serial.print(normal6->getPertinence());
    Serial.print(", kelainan6-> ");
    Serial.println(kelainan6->getPertinence());

    float output1 = fuzzy->defuzzify(1);

    Serial.println("Output: ");
    Serial.print("normalis-> ");
    Serial.print(normalis->getPertinence());
    Serial.print(", lordosis-> ");
    Serial.print(lordosis->getPertinence());
    Serial.print(", kifosis-> ");
    Serial.print(kifosis->getPertinence());
    Serial.print(", skoliosis-> ");
    Serial.println(skoliosis->getPertinence());
}
