$fn = 64;

cylRadius = 1.75;
symbolRadius = cylRadius * 1.25;

symbolOffset = cylRadius - symbolRadius / 7 - 0.2;

module arc(radius, a1, a2) {
    step = (a2 - a1) / $fn;
    points = [
        for(a = [a1:step:a2]) [radius * cos(a), radius * sin(a)]
    ];
    polygon(concat([[0, 0]], points));
}

module segment(radius, a1, a2, thickness) {
    difference() {
        arc(radius, a1, a2);
        arc(radius - thickness, a1 - 0.01, a2 + 0.01);
    }
}

module wifiSign() {
    circle(r = symbolRadius / 7);
    segment(radius = symbolRadius, a1 = 45, a2 = 135, thickness = symbolRadius / 7);
    segment(radius = 5 * symbolRadius / 7, a1 = 45, a2 = 135, thickness = symbolRadius / 7);
    segment(radius = 3 * symbolRadius / 7, a1 = 45, a2 = 135, thickness = symbolRadius / 7);
}

translate([0, -symbolOffset, 0.5 + 2.5])
    linear_extrude(height=1, center = true, convexity = 10, twist = 0, slices = 20, scale = 1.0) {
        wifiSign();
    }
cylinder(r = cylRadius, h = 2.5);
