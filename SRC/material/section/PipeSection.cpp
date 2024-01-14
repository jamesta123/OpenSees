/* ******************************************************************
***
**    OpenSees - Open System for Earthquake Engineering Simulation **
**          Pacific Earthquake Engineering Research Center **
** **
** **
** (C) Copyright 1999, The Regents of the University of California **
** All Rights Reserved. **
** **
** Commercial use of this program without express permission of the **
** University of California, Berkeley, is strictly prohibited.  See **
** file 'COPYRIGHT'  in main directory for information on usage and **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES. **
** **
** Developed by: **
**   Frank McKenna (fmckenna@ce.berkeley.edu) **
**   Gregory L. Fenves (fenves@ce.berkeley.edu) **
**   Filip C. Filippou (filippou@ce.berkeley.edu) **
** **
** ******************************************************************
*/

// Minjie

#include <PipeSection.h>

static double pi = 3.141592653589793;

void *OPS_PipeSection(void) {
    // line 9270
    // check inputs
    if (OPS_GetNumRemainingInputArgs() < 5) {
        opserr << "Invalid #args,  want: section Pipe "
                  "tag? do? t? alphaV? w? <rho?>\n";
        return 0;
    }

    // get tag
    int iData[1];
    int numData = 1;
    if (OPS_GetIntInput(&numData, iData) < 0) {
        opserr << "WARNING invalid tag  for pipe section\n";
        return 0;
    }

    // get data: do, t, alphaV, w
    double data[4];
    numData = 4;
    if (OPS_GetDoubleInput(&numData, data) < 0) {
        opserr << "WARNING: invalid data for pipe section\n";
        return 0;
    }
    if (data[0] <= 1e-8) {
        opserr << "WARNING: outside diameter is zero or negative\n";
        return 0;
    }
    if (data[1] <= 1e-8) {
        opserr << "WARNING: thickness is zero or negative\n";
        return 0;
    }
    if (data[3] <= 0.0) {
        opserr << "WARNING: weight is zero or negative\n";
        return 0;
    }

    double rho = 0.0;
    numData = 1;
    if (OPS_GetNumRemainingInputArgs() > 0) {
        if (OPS_GetDoubleInput(&numData, data) < 0) {
            opserr << "WARNING: invalid rho for pipe section\n";
            return 0;
        }
    }

    auto *sect = new PipeSection(iData[0], data[0], data[1], data[2],
                                 data[3], rho);

    return sect;
}

PipeSection::PipeSection(int tag, double d, double t, double a,
                         double w, double r)
    : SectionForceDeformation(tag, SEC_TAG_PipeSection),
      dout(d),
      thk(t),
      alphaV(a),
      wgt(w),
      rho(r),
      e(),
      mat(),
      code(2) {
    if (rho <= 1e-12) {
        rho = wgt / 386.4;
    }
    rout = dout * 0.5;
    rin = rout - thk;
    double rout2 = rout * rout;
    double rin2 = rin * rin;
    Ax = pi * (rout2 - rin2);
    if (Ax <= 0) {
        opserr << "WARNING: AREA <= 0\n";
    }
    Iy = 0.25 * pi * (rout2 * rout2 - rin2 * rin2);
    if (Iy <= 0) {
        opserr << "WARNING: Iy <= 0\n";
    }
    if (alphaV < 1e-8) {
        double dum2 = 4.0 * (rout2 * rout - rin2 * rin) / 3.0;
        double dum3 = (rout2 + rin2) * thk;
        if (dum3 < 1e-8) {
            opserr << "WARNING: (ro^2+ri^2) * t < 1e-8. AlphaV "
                      "is set to 100.\n";
            alphaV = 100.0;
        } else {
            alphaV = dum2 / dum3;
        }
    }
    Iz = Iy;
    Jx = 2 * Iy;
    Ay = Ax / alphaV;
    Az = Ay;
}
PipeSection::~PipeSection(void) {}

int PipeSection::commitState(void) { return 0; }
int PipeSection::revertToLastCommit(void) { return 0; }
int PipeSection::revertToStart(void) { return 0; }

const char *PipeSection::getClassType(void) const {
    return "PipeSection";
};

int PipeSection::setTrialSectionDeformation(const Vector &) {
    return 0;
}
const Vector &PipeSection::getSectionDeformation(void) { return e; }

const Vector &PipeSection::getStressResultant(void) { return e; }
const Matrix &PipeSection::getSectionTangent(void) { return mat; }
const Matrix &PipeSection::getInitialTangent(void) { return mat; }
const Matrix &PipeSection::getSectionFlexibility(void) { return mat; }
const Matrix &PipeSection::getInitialFlexibility(void) { return mat; }

SectionForceDeformation *PipeSection::getCopy(void) {
    auto *theCopy =
        new PipeSection(this->getTag(), this->dout, this->thk,
                        this->alphaV, this->wgt, this->rho);
    theCopy->parameterID = this->parameterID;
    return theCopy;
}
const ID &PipeSection::getType(void) { return code; }
int PipeSection::getOrder(void) const { return 2; }

int PipeSection::sendSelf(int commitTag, Channel &theChannel) {
    return 0;
}
int PipeSection::recvSelf(int commitTag, Channel &theChannel,
                          FEM_ObjectBroker &theBroker) {
    return 0;
}

void PipeSection::Print(OPS_Stream &s, int flag) {}

int PipeSection::setParameter(const char **argv, int argc,
                              Parameter &param) {
    return -1;
}
int PipeSection::updateParameter(int parameterID, Information &info) {
    return -1;
}
int PipeSection::activateParameter(int parameterID) {
    this->parameterID = parameterID;
}
const Vector &PipeSection::getStressResultantSensitivity(
    int gradIndex, bool conditional) {
    return e;
}
const Matrix &PipeSection::getSectionTangentSensitivity(
    int gradIndex) {
    return mat;
}
const Matrix &PipeSection::getInitialTangentSensitivity(
    int gradIndex) {
    return mat;
}
const Matrix &PipeSection::getSectionFlexibilitySensitivity(
    int gradIndex) {
    return mat;
}
const Matrix &PipeSection::getInitialFlexibilitySensitivity(
    int gradIndex) {
    return mat;
}
