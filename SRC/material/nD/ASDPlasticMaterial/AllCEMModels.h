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

#include "ConstitutiveModels/VonMisesLinearHardening.h"
#include "ConstitutiveModels/DruckerPragerLinearHardening.h"
#include "ConstitutiveModels/DruckerPragerVonMisesLinearHardening.h"
#include "ConstitutiveModels/DruckerPragerArmstrongFrederickLE.h"
#include "ConstitutiveModels/DruckerPragerArmstrongFrederickNE.h"
#include "ConstitutiveModels/DruckerPragerNonAssociateLinearHardening.h"
#include "ConstitutiveModels/DruckerPragerNonAssociateArmstrongFrederick.h"
#include "ConstitutiveModels/VonMisesArmstrongFrederick.h"
#include "ConstitutiveModels/CamClayLT.h"
#include "ConstitutiveModels/RoundedMohrCoulomb.h"
#include "ConstitutiveModels/sanisand2004.h"