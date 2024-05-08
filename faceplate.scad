$fn=32;

switchHoleWidth=14.4;
switchHoleDepth=14.4;
plateThickness=1.5;
facePlateTop=5;

switchesX = 5;
switchesY = 5;

switchDistX=19.21;
switchDistY=19.22;

switchOuterMargin=4;

plateOuterMargin=2;

facePlateWidth = (switchesX - 1) * switchDistX + switchHoleWidth + 2 * switchOuterMargin;
facePlateDepth = (switchesY - 1) * switchDistY + switchHoleDepth + 2 * switchOuterMargin;

difference() {
    union() {
        translate([0, 0, facePlateTop - plateThickness/2])
            difference() {
                union() {
                    // Main plate
                    cube([facePlateWidth + 2 * plateOuterMargin, facePlateDepth + 2 * plateOuterMargin, plateThickness], center=true);
                    for (x = [0 : switchesX - 2]) {
                        for (y = [0 : switchesY - 2]) {
                            translate([x * switchDistX + switchHoleWidth + (switchDistX - switchHoleWidth)/2 - facePlateWidth/2 + switchOuterMargin,
                                       y * switchDistY + switchHoleDepth + (switchDistY - switchHoleDepth)/2 - facePlateDepth/2 + switchOuterMargin, -(facePlateTop - plateThickness/2 - 0.01)])
                                cylinder(r=1, h=facePlateTop);
                        }
                    }
                }
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
                // Screw mount hole, bottom left
                translate([-(facePlateWidth + 2 * plateOuterMargin)/2, -(facePlateDepth + 2 * plateOuterMargin)/2 - 1, -5])
                    cylinder(r=3.5, h=10);
            }
        // Rim
        translate([0, -(facePlateDepth/2 - plateThickness/2), facePlateTop/2])
            cube([facePlateWidth, plateThickness, facePlateTop], center = true);
        translate([-(facePlateWidth/2 - plateThickness/2), 0, facePlateTop/2])
            cube([plateThickness, facePlateDepth, facePlateTop], center = true);
        translate([(facePlateWidth/2 - plateThickness/2), 0, facePlateTop/2])
            cube([plateThickness, facePlateDepth, facePlateTop], center = true);
        translate([0, facePlateDepth/2 - plateThickness/2, facePlateTop/2])
            cube([facePlateWidth, plateThickness, facePlateTop], center=true);

    }
}
