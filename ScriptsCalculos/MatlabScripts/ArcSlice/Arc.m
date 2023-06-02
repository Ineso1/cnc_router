classdef Arc
    properties
        a;
        b;
        r;
    end
    
    methods
        function obj = Arc(a, b, r)
            obj.a = a;
            obj.b = b;
            obj.r = r;
        end
        
        function arcLength = length(obj)
            arcLength = obj.r * acos(1 - ((obj.a.distance(obj.b))^2) / (2 * obj.r^2));
        end
        
        function slices = sliceArc(obj, spacing)
            numSegments = ceil(obj.length() / spacing);
            angleIncrement = obj.calculateAngleIncrement();
            
            slices = Point.empty(numSegments, 0);
            currentAngle = obj.a.angle();
            
            for i = 1:numSegments
                x = obj.a.x + obj.r * cos(currentAngle);
                y = obj.a.y + obj.r * sin(currentAngle);
                z = obj.a.z;  % Assuming the arc lies in the xy plane
                
                slices(i) = Point(x, y, z);
                
                currentAngle = currentAngle + angleIncrement;
            end
        end
        
        function angleIncrement = calculateAngleIncrement(obj)
            angleIncrement = (obj.b.angle() - obj.a.angle()) / (obj.b - obj.a).magnitude();
        end
    end
end
