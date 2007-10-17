#include "G4GDMLEvaluator.hh"

G4GDMLEvaluator::G4GDMLEvaluator() {

   eval.clear();
   eval.setStdMath();
   eval.setSystemOfUnits(1.e+3,1./1.60217733e-25,1.e+9,1./1.60217733e-10,1.0,1.0,1.0);
}

G4GDMLEvaluator::~G4GDMLEvaluator() {
}

G4GDMLEvaluator *G4GDMLEvaluator::GetInstance() {

   static G4GDMLEvaluator instance;

   return &instance;
}

bool G4GDMLEvaluator::RegisterConstant(const std::string &name,double value) {

   eval.setVariable(name.c_str(),value);

   return true;
}

bool G4GDMLEvaluator::Evaluate(double& value,const std::string& expression,const std::string& unit) {

   double _expression = 0.0;
   double _unit = 1.0;

   if (expression != "") {
   
      _expression = eval.evaluate(expression.c_str());

      if (eval.status() != HepTool::Evaluator::OK) {

         eval.print_error();
         return false;
      }
   }

   if (unit != "") {
   
      _unit = eval.evaluate(unit.c_str());

      if (eval.status() != HepTool::Evaluator::OK) {

         eval.print_error();
         return false;
      }
   }

   value = _expression*_unit;

   return true;
}
