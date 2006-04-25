// $Id: Step.hh,v 1.2 2006-04-25 07:54:28 kmura Exp $
// ====================================================================
//   Step.hh
//
//                                              2004 Q
// ====================================================================
#ifndef STEP_H
#define STEP_H

// ====================================================================
//
// class definition
//
// ====================================================================

class Step {
private:
  double x;

public:
  Step();
  ~Step();

  void SetX(double x0);
  double GetX() const;

};

// ====================================================================
// inline functions
// ====================================================================
inline void Step::SetX(double x0) { x= x0; }
inline double Step::GetX() const { return x; }

#endif
