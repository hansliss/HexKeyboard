$fn=255;

include <dimensions.scad>

module screwStandoffLow() {
    translate([0, 0, caseWallThickness / 2])
        rotate([caseTopRimAngle, 0, 0])
            cylinder(r=4, h=4);
}

module screwStandoffHigh() {
    translate([0, 0, caseWallThickness / 2])
        rotate([caseTopRimAngle, 0, 0])
            cylinder(r=4, h=22.8);
}

module screwHoleLow() {
    translate([0, 0, -caseWallThickness / 2])
        rotate([caseTopRimAngle, 0, 0])
            union() {
                cylinder(r=1.5, h=4 + caseWallThickness + 0.2);
                translate([0, 0, -(caseWallThickness / 2 + 0.2)])
                    cylinder(r=3, h=4);
            }
}

module screwHoleHigh() {
    translate([0, 0, -caseWallThickness / 2])
        rotate([caseTopRimAngle, 0, 0])
            union() {
                cylinder(r=1.5, h=22.8 + caseWallThickness + 0.2);
                translate([0, 0, -(caseWallThickness / 2 + 0.2)])
                    cylinder(r=3, h=22.8);
            }
}

difference() {
    union() {
        cube([caseWidth, caseDepth / cos(caseTopRimAngle), caseWallThickness], center = true);
        intersection() {
            translate([0, 0, caseWallThickness - 0.01])
                cube([caseWidth - (2 * caseWallThickness + 0.2), 5 + caseDepth - (2 * caseWallThickness + 0.2), caseWallThickness], center = true);
            translate([0, 0, caseWallThickness / 2])
                rotate([caseTopRimAngle, 0, 0])
                    cube([caseWidth - (2 * caseWallThickness + 0.2), (caseDepth - (2 * caseWallThickness + 0.2)), 50], center = true);
        }
        // 4 x top screw standoff
        translate([(pcbAbsBLHoleX - pcbCX), - (pcbAbsBLHoleY - pcbCY) / cos(caseTopRimAngle), 0])
            screwStandoffLow();
        translate([(pcbAbsTLHoleX - pcbCX), - (pcbAbsTLHoleY - pcbCY) / cos(caseTopRimAngle), 0])
            union() {
                screwStandoffHigh();
                translate([0, 0, caseWallThickness * 1.5 - 0.01])
                difference() {
                    cylinder(r = 4 + caseTopStandoffSkirtRadius, h = caseTopStandoffSkirtRadius);
                    translate([0, 0, caseTopStandoffSkirtRadius])
                        rotate_extrude(convexity = 10)
                            translate([4 + caseTopStandoffSkirtRadius, 0, 0])
                                circle(r = caseTopStandoffSkirtRadius);
                }
            }
        translate([(pcbAbsTRHoleX - pcbCX), - (pcbAbsTRHoleY - pcbCY) / cos(caseTopRimAngle), 0])
            union() {
                screwStandoffHigh();
                translate([0, 0, caseWallThickness * 1.5 - 0.01])
                difference() {
                    cylinder(r = 4 + caseTopStandoffSkirtRadius, h = caseTopStandoffSkirtRadius);
                    translate([0, 0, caseTopStandoffSkirtRadius])
                        rotate_extrude(convexity = 10)
                            translate([4 + caseTopStandoffSkirtRadius, 0, 0])
                                circle(r = caseTopStandoffSkirtRadius);
                }
            }
        translate([(pcbAbsBRHoleX - pcbCX), - (pcbAbsBRHoleY - pcbCY) / cos(caseTopRimAngle), 0])
            screwStandoffLow();
    }
    // 4 x top screw standoff
    translate([(pcbAbsBLHoleX - pcbCX), - (pcbAbsBLHoleY - pcbCY) / cos(caseTopRimAngle), 0])
        screwHoleLow();
    translate([(pcbAbsTLHoleX - pcbCX), - (pcbAbsTLHoleY - pcbCY) / cos(caseTopRimAngle), 0])
        screwHoleHigh();
    translate([(pcbAbsTRHoleX - pcbCX), - (pcbAbsTRHoleY - pcbCY) / cos(caseTopRimAngle), 0])
        screwHoleHigh();
    translate([(pcbAbsBRHoleX - pcbCX), - (pcbAbsBRHoleY - pcbCY) / cos(caseTopRimAngle), 0])
        screwHoleLow();
    
}