$fn=16;

include <dimensions.scad>

module screwStandoff() {
    translate([0, 0, - (caseTopStandoffHeight + caseWallThickness / 2 - 0.01)])
        union() {
            difference() {
                cylinder(r = caseTopStandoffOuterRadius, h = caseTopStandoffHeight + 0.01);
                translate([0, 0, -0.01])
                    cylinder(r = caseTopStandoffHoleRadius, h = caseTopStandoffHeight + 0.02);
            }
            translate([0, 0, caseTopStandoffHeight - caseTopStandoffSkirtRadius])
                difference() {
                    cylinder(r = caseTopStandoffOuterRadius + caseTopStandoffSkirtRadius, h = caseTopStandoffSkirtRadius);
                    
                    rotate_extrude(convexity = 10)
                        translate([caseTopStandoffOuterRadius + caseTopStandoffSkirtRadius, 0, 0])
                            circle(r = caseTopStandoffSkirtRadius);
                }
        }
}

union() {
    difference() {
        union() {
            // Top plate
            translate([caseOffsetX, caseOffsetY])
                cube([caseWidth, caseDepth, caseWallThickness], center = true);

                // 4 x top screw standoff
            translate([pcbAbsBLHoleX - pcbCX, - (pcbAbsBLHoleY - pcbCY), 0])
                screwStandoff();
            translate([pcbAbsTLHoleX - pcbCX, - (pcbAbsTLHoleY - pcbCY), 0])
                screwStandoff();
            translate([pcbAbsTRHoleX - pcbCX, - (pcbAbsTRHoleY - pcbCY), 0])
                screwStandoff();
            translate([pcbAbsBRHoleX - pcbCX, - (pcbAbsBRHoleY - pcbCY), 0])
                screwStandoff();

            // 2 x top ZIP screw standoff
            translate([pcbAbsZifTopHoleX - pcbCX, - (pcbAbsZifTopHoleY - pcbCY), 0])
                screwStandoff();
            translate([pcbAbsZifBottomHoleX - pcbCX, - (pcbAbsZifBottomHoleY - pcbCY), 0])
                screwStandoff();
        }
        // Keypad opening
        translate([pcbAbsKeypadCenterX - pcbCX, - (pcbAbsKeypadCenterY - pcbCY), 0])
            cube([caseSwitchPlateOpeningWidth, caseSwitchPlateOpeningDepth, caseWallThickness + 10], center = true);
        
        // Display opening
        translate([pcbAbsDisplayCenterX - pcbCX, - (pcbAbsDisplayCenterY - pcbCY), 00])
            cube([displayWidth, displayDepth, caseWallThickness + 0.2], center = true);
        
        // ZIF socket opening
        translate([pcbAbsZifHoleCenterX - pcbCX, -(pcbAbsZifHoleCenterY - pcbCY), 0])
            cube([caseZifHoleWidth, caseZifHoleDepth, caseWallThickness + 0.02], center = true);
    }
    
    
    translate([caseOffsetX, caseOffsetY])
    difference() {
        union() {
            translate([-(caseWidth / 2 - caseWallThickness / 2), 0, -(caseWallThickness / 2 + caseTopRimHeightUpper / 2 - 0.01 / 2)])
                cube([caseWallThickness, caseDepth, caseTopRimHeightUpper + 0.01], center = true);
            translate([(caseWidth / 2 - caseWallThickness / 2), 0, -(caseWallThickness / 2 + caseTopRimHeightUpper / 2 - 0.01 / 2)])
                cube([caseWallThickness, caseDepth, caseTopRimHeightUpper + 0.01], center = true);
            translate([0, -(caseDepth / 2 - caseWallThickness / 2), -(caseWallThickness / 2 + caseTopRimHeightUpper / 2 - 0.01 / 2)])
                cube([caseWidth, caseWallThickness, caseTopRimHeightUpper + 0.01], center = true);
            difference() {
                translate([0, (caseDepth / 2 - caseWallThickness / 2), -(caseWallThickness / 2 + caseTopRimHeightUpper / 2 - 0.01 / 2)])
                    cube([caseWidth, caseWallThickness, caseTopRimHeightUpper + 0.01], center = true);
                translate([
                    pcbAbsUsbX - pcbCX,
                    caseDepth/2,
                    -(caseWallThickness / 2 + caseTopStandoffHeight + pcbHeight + caseUsbHoleHeight / 2)
                ])
                    cube([caseUsbHoleWidth, 20, caseUsbHoleHeight], center=true);
                translate([
                    pcbAbsSdX - pcbCX,
                    caseDepth/2,
                    -(caseWallThickness / 2 + caseTopStandoffHeight + pcbHeight + caseSdHoleHeight / 2)
                ])
                    cube([caseSdHoleWidth, 20, caseSdHoleHeight], center=true);
            }
        };
        // Cut off the bottom at an angle
        translate([0, 0, -(caseTopRimHeightUpper * 3 / 2 + caseTopRimOffset / 2 + caseTopRimHeightLower)])
            rotate([-caseTopRimAngle, 0, 0])
                cube([caseWidth + 5, caseDepth + 20, caseTopRimHeightUpper * 3], center = true);
    }

    // 4 x inner rim to meet with keypad faceplate
    translate([
        pcbAbsKeypadCenterX - pcbCX + 
        -(caseSwitchPlateOpeningWidth / 2 + caseWallThickness / 2),
        -(pcbAbsKeypadCenterY - pcbCY),
        -(caseWallThickness / 2 + caseSwitchPlateOpeningRimHeight / 2 - 0.015)
    ])
        cube([
            caseWallThickness,
            caseSwitchPlateOpeningDepth + 2 * caseWallThickness,
            caseSwitchPlateOpeningRimHeight + 0.01
        ], center = true);
    translate([
        pcbAbsKeypadCenterX - pcbCX + 
        (caseSwitchPlateOpeningWidth / 2 + caseWallThickness / 2),
        -(pcbAbsKeypadCenterY - pcbCY),
        -(caseWallThickness / 2 + caseSwitchPlateOpeningRimHeight / 2 - 0.015)
    ])
        cube([
            caseWallThickness,
            caseSwitchPlateOpeningDepth + 2 * caseWallThickness,
            caseSwitchPlateOpeningRimHeight + 0.01
        ], center = true);
    translate([
        pcbAbsKeypadCenterX - pcbCX,
        -(pcbAbsKeypadCenterY - pcbCY)
        + (caseSwitchPlateOpeningDepth / 2 + caseWallThickness / 2),
        -(caseWallThickness / 2 + caseSwitchPlateOpeningRimHeight / 2 - 0.015)
    ])
        cube([
            caseSwitchPlateOpeningWidth + 2 * caseWallThickness,
            caseWallThickness,
            caseSwitchPlateOpeningRimHeight + 0.01
        ], center = true);
    translate([
        pcbAbsKeypadCenterX - pcbCX,
        -(pcbAbsKeypadCenterY - pcbCY)
        - (caseSwitchPlateOpeningDepth / 2 + caseWallThickness / 2),
        -(caseWallThickness / 2 + caseSwitchPlateOpeningRimHeight / 2 - 0.015)
    ])
        cube([
            caseSwitchPlateOpeningWidth + 2 * caseWallThickness,
            caseWallThickness,
            caseSwitchPlateOpeningRimHeight + 0.01
        ], center = true);
        

}