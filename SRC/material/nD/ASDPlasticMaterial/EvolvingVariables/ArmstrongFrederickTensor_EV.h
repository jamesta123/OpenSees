/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// Original implementation: José Abell (UANDES), Massimo Petracca (ASDEA)
//
// ASDPlasticMaterial
//
// Fully general templated material class for plasticity modeling


#ifndef ArmstrongFrederickTensor_EV_H
#define ArmstrongFrederickTensor_EV_H

#include "../EvolvingVariable.h"
#include "../ClassicElastoplasticityGlobals.h" // Defines indices i,j,k,l,m,n,p,q,r,s and the kronecker_delta.

class ArmstrongFrederickTensor_EV;
template< >
struct requires_hardening_saturation_limit_check<ArmstrongFrederickTensor_EV>
{
    static const bool requires = true;
};

class ArmstrongFrederickTensor_EV : public EvolvingVariable<DTensor2, ArmstrongFrederickTensor_EV> //CRTP on ArmstrongFrederickTensor_EV
{
public:

    ArmstrongFrederickTensor_EV( double ha_, double cr_);
    ArmstrongFrederickTensor_EV( double ha_, double cr_, DTensor2& alpha0);

    const DTensor2& getDerivative(const DTensor2 &depsilon,
                                  const DTensor2 &m,
                                  const DTensor2& stress) const;
    void check_hardening_saturation_limit(DTensor2& backstress, DTensor2 const& plasticFlow_m );
    int sendSelf(int commitTag, Channel &theChannel);
    int receiveSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

private:
    double ha, cr;
    static DTensor2 derivative;     // Needs to be static so multiple instances only do one malloc call and we can return a const-reference
};



#endif