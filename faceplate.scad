$fn=256;

switchHoleWidth=14.4;
switchHoleDepth=14.4;
plateThickness=1.5;
facePlateTop=5;

switchesX = 3;
switchesY = 4;

switchDistX=19.21;
switchDistY=19.22;

switchOuterMargin=4;

holeDistXFromSwitchCenter = 9.85;
bottomHoleDistYFromSwitchCenter = 11;
holeYDistance=91.2;

// The keys are, sadly, not exactly centered, but this design is fixed now so we'll compensate instead.
holeRightXPos = 29.41; //((switchesX - 1) / 2) * switchDistX + holeDistXFromSwitchCenter;
holeLeftXPos = -holeRightXPos;
bottomHoleYPos = -(((switchesY - 1) / 2) * switchDistY + bottomHoleDistYFromSwitchCenter);
topHoleYPos = bottomHoleYPos + holeYDistance;

topEdgeGap=36;

holeDia=3.5;
holeOuterDia=6;

facePlateWidth = (switchesX - 1) * switchDistX + switchHoleWidth + 2 * switchOuterMargin;
facePlateDepth = (switchesY - 1) * switchDistY + switchHoleDepth + 2 * switchOuterMargin;

topEdgePartWidth=(facePlateWidth - topEdgeGap) / 2;

topSparWidth=10;
topSparDepth=holeYDistance - facePlateDepth - holeDia/2;

difference() {
    union() {
        translate([0, 0, facePlateTop - plateThickness/2])
            difference() {
                // Main plate
                cube([facePlateWidth, facePlateDepth, plateThickness], center=true);
                // Switch holes
                union() {
                    for (x = [0 : switchesX - 1]) {
                        for (y = [0 : switchesY - 1]) {
                            translate([x * switchDistX + switchHoleWidth/2 - facePlateWidth/2 + switchOuterMargin,
                                       y * switchDistY + switchHoleDepth/2 - facePlateDepth/2 + switchOuterMargin, 0])
                                cube([switchHoleWidth, switchHoleDepth, plateThickness + 0.02], center=true);
                        }
                    }
                }
            }
        // Inner "rim" section around bottom holes
        intersection() {
            translate([0, 0, facePlateTop/2])
                cube([facePlateWidth, facePlateDepth, facePlateTop], center=true);
            union() {
                translate([holeLeftXPos, bottomHoleYPos, 0])
                    cylinder(r=holeOuterDia/2 + plateThickness, h=facePlateTop);
                translate([holeRightXPos, bottomHoleYPos, 0])
                    cylinder(r=holeOuterDia/2 + plateThickness, h=facePlateTop);
            }
        }
        // Rim
        translate([0, -(facePlateDepth/2 - plateThickness/2), facePlateTop/2])
            cube([facePlateWidth, plateThickness, facePlateTop], center = true);
        translate([-(facePlateWidth/2 + plateThickness/2), 0, facePlateTop/2])
            cube([plateThickness, facePlateDepth, facePlateTop], center = true);
        translate([(facePlateWidth/2 + plateThickness/2), 0, facePlateTop/2])
            cube([plateThickness, facePlateDepth, facePlateTop], center = true);
        translate([(facePlateWidth/2 - topEdgePartWidth/2), facePlateDepth/2 - plateThickness/2, facePlateTop/2])
            cube([topEdgePartWidth, plateThickness, facePlateTop], center=true);
        translate([-(facePlateWidth/2 - topEdgePartWidth/2), facePlateDepth/2 - plateThickness/2, facePlateTop/2])
            cube([topEdgePartWidth, plateThickness, facePlateTop], center=true);
        // shaped connectors between main body and top holes,
        // with strengthening spars
        translate([(facePlateWidth/2 - topSparWidth/2), facePlateDepth/2 + topSparDepth/2, plateThickness/2])
            union() {
                difference() {
                    cube([topSparWidth, topSparDepth, plateThickness], center=true);
                    translate([-(topSparDepth*0.71 + holeDia), topSparDepth*0.61, 0])
                        rotate([0, 0, 45])
                            cube([topSparDepth*2, topSparDepth*2, plateThickness + 0.02], center=true);
                }
                translate([2, -1, plateThickness/2])
                    rotate([-8, 0, -10])
                        cube([plateThickness, topSparDepth, plateThickness], center=true);
            }
        translate([-(facePlateWidth/2 - topSparWidth/2), facePlateDepth/2 + topSparDepth/2, plateThickness/2])
            union() {
                difference() {
                    cube([topSparWidth, topSparDepth, plateThickness], center=true);
                    translate([topSparDepth*0.71 + holeDia, topSparDepth*0.61, 0])
                        rotate([0, 0, -45])
                            cube([topSparDepth*2, topSparDepth*2, plateThickness + 0.02], center=true);
                }
                translate([-2, -1, plateThickness/2])
                    rotate([-8, 0, 10])
                        cube([plateThickness, topSparDepth, plateThickness], center=true);
            }
    }
    // Cutout for bottom holes
    union() {
        translate([holeLeftXPos, bottomHoleYPos, -0.01])
            cylinder(r = holeOuterDia/2, h = facePlateTop + 0.02);
        translate([holeRightXPos, bottomHoleYPos, -0.01])
            cylinder(r = holeOuterDia/2, h = facePlateTop + 0.02);
    }
}
// Bottom mounting holes
translate([holeLeftXPos, bottomHoleYPos, 0])
    difference() {
        cylinder(r=holeOuterDia/2, h=plateThickness);
        translate([0, 0, -0.01])
            cylinder(r=holeDia/2, h=plateThickness + 0.02);
    }
translate([holeRightXPos, bottomHoleYPos, 0])
    difference() {
        cylinder(r=holeOuterDia/2, h=plateThickness);
        translate([0, 0, -0.01])
            cylinder(r=holeDia/2, h=plateThickness + 0.02);
    }
// Top mounting holes
translate([holeLeftXPos, topHoleYPos, 0])
    difference() {
        cylinder(r=holeOuterDia/2, h=plateThickness);
        translate([0, 0, -0.01])
            cylinder(r=holeDia/2, h=plateThickness + 0.02);
    }
translate([holeRightXPos, topHoleYPos, 0])
    difference() {
        cylinder(r=holeOuterDia/2, h=plateThickness);
        translate([0, 0, -0.01])
            cylinder(r=holeDia/2, h=plateThickness + 0.02);
    }