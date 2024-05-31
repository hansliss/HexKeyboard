facePlateSwitchHoleWidth=14.4;
facePlateSwitchHoleDepth=14.4;
facePlateThickness=1.5;
facePlateTop=5;

facePlateSwitchesX = 5;
facePlateSwitchesY = 5;

facePlateSwitchDistX=19.21;
facePlateSwitchDistY=19.22;

facePlateSwitchOuterMargin=4;

facePlateOuterMargin=2;

facePlateWidth = (facePlateSwitchesX - 1) * facePlateSwitchDistX + facePlateSwitchHoleWidth + 2 * facePlateSwitchOuterMargin;
facePlateDepth = (facePlateSwitchesY - 1) * facePlateSwitchDistY + facePlateSwitchHoleDepth + 2 * facePlateSwitchOuterMargin;

facePlateDistanceFromBLScrewHoleX = 0;
facePlateDistanceFromBLScrewHoleY = 1;

pcbAbsLeftEdgeX = 31.94;
pcbAbsTopEdgeY = 38.2;
pcbAbsRightEdgeX = 164.1;
pcbAbsBottomEdgeY = 182.34;

pcbAbsTLHoleX = 36.94;
pcbAbsTLHoleY = 43.2;

pcbAbsBLHoleX = 36.94;
pcbAbsBLHoleY = 177.34;

pcbAbsTRHoleX = 159.1;
pcbAbsTRHoleY = 43.2;

pcbAbsBRHoleX = 159.1;
pcbAbsBRHoleY = 177.34;

pcbKeySwitchCenterOffsetX = -2.54;
pcbKeySwitchCenterOffsetY = 5.08;

pcbAbsKeypadCenterX = 90.555 + pcbKeySwitchCenterOffsetX;
pcbAbsKeypadCenterY = 120.25 + pcbKeySwitchCenterOffsetY;

pcbAbsZifTopHoleX = 152.82;
pcbAbsZifTopHoleY = 83.49;

pcbAbsZifBottomHoleX = 152.82;
pcbAbsZifBottomHoleY = 126.96;

pcbAbsZifHoleCenterX = 152.9;
pcbAbsZifHoleCenterY = 105.05;

pcbAbsUsbX = 108.6025;
pcbAbsSdX = 81.94;

pcbAbsDisplayCenterX = 40 + 51.33;
pcbAbsDisplayCenterY = 48.935;

pcbCX = (pcbAbsLeftEdgeX + pcbAbsRightEdgeX) / 2;
pcbCY = (pcbAbsTopEdgeY + pcbAbsBottomEdgeY) / 2;

pcbWidth = pcbAbsRightEdgeX - pcbAbsLeftEdgeX;
pcbDepth = pcbAbsBottomEdgeY - pcbAbsTopEdgeY;
pcbHeight = 1.6;

displayWidth = 61;
displayDepth = 14.5;

caseWallThickness = 1.8;

caseSwitchMargin = 3.5;
caseSwitchPlateOpeningWidth = (facePlateSwitchesX - 1) * facePlateSwitchDistX + facePlateSwitchHoleWidth + 2 * caseSwitchMargin;
caseSwitchPlateOpeningDepth = (facePlateSwitchesY - 1) * facePlateSwitchDistY + facePlateSwitchHoleDepth + 2 * caseSwitchMargin;

caseSwitchPlateOpeningRimHeight = 3;

caseSwitchPlateOpeningPosX = pcbAbsKeypadCenterX - pcbCX;
caseSwitchPlateOpeningPosy = pcbAbsKeypadCenterY - pcbCY;

caseMarginTop = 0.2;
caseMarginBottom = 1.2;
caseMarginLeft = 1.2;
caseMarginRight = 1.2;
caseWidth = pcbWidth + caseMarginLeft + caseMarginRight + 2 * caseWallThickness;
caseDepth = pcbDepth + caseMarginTop + caseMarginBottom + 2 * caseWallThickness;
caseOffsetY = (caseMarginTop - caseMarginBottom)/2;
caseOffsetX = (caseMarginRight - caseMarginLeft)/2;

caseTopStandoffHeight = caseSwitchPlateOpeningRimHeight + facePlateTop;
caseTopStandoffHoleRadius = 1.25;
caseTopStandoffOuterRadius = 3;
caseTopStandoffSkirtRadius = 2;

caseZifHoleWidth = 15.5;
caseZifHoleDepth = 33;

caseTopRimAngle = 8;
caseTopRimOffset = tan(caseTopRimAngle) * caseDepth;
caseTopRimHeightLower = caseTopStandoffHeight + 6;
caseTopRimHeightUpper = caseTopStandoffHeight + 6 + caseTopRimOffset;

caseUsbHoleWidth = 10;
caseUsbHoleHeight = 4.5;

caseSdHoleWidth = 15.5;
caseSdHoleHeight = 2.5;

caseBottomTallStandoffHeight = 22.8;
caseBottomShortStandoffHeight = 4;
caseBottomStandoffOuterRadius = 4;
caseBottomStandoffInnerRadius = 3;
caseBottomStandoffHoleRadius = 1.6;
caseBottomStandoffSkirtRadius = caseTopStandoffSkirtRadius;
