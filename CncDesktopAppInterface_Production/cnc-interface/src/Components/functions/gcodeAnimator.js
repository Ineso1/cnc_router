function interpolateArc(startX, startY, startZ, endX, endY, endZ, centerX, centerY, radius, clockwise, timeStep) {
  const startAngle = Math.atan2(startY - centerY, startX - centerX);
  const endAngle = Math.atan2(endY - centerY, endX - centerX);

  let angle;
  if (clockwise) {
    if (startAngle < 0) {
      angle = 2 * Math.PI - Math.abs(startAngle) + Math.abs(endAngle);
    } else {
      angle = endAngle - startAngle;
    }
  } else {
    if (startAngle > 0) {
      angle = 2 * Math.PI - startAngle + endAngle;
    } else {
      angle = startAngle - endAngle;
    }
  }

  const numSteps = Math.ceil(angle * radius / timeStep);

  const coordinates = [];

  for (let i = 0; i <= numSteps; i++) {
    const t = i / numSteps;
    const currentAngle = startAngle + t * angle;

    const currentX = centerX + radius * Math.cos(currentAngle);
    const currentY = centerY + radius * Math.sin(currentAngle);
    const currentZ = startZ + t * (endZ - startZ);

    coordinates.push({ x: currentX, y: currentY, z: currentZ });
  }

  return coordinates;
}



function animateGCode(x0, y0, z0, gcodeLine, timeStep) {
  
    const xRegex = /X(-?[\d.]+)/;
    const yRegex = /Y(-?[\d.]+)/;
    const zRegex = /Z(-?[\d.]+)/;
    const iRegex = /I(-?[\d.]+)/;
    const jRegex = /J(-?[\d.]+)/;
    const rRegex = /R(-?[\d.]+)/;

    const xMatch = gcodeLine.match(xRegex);
    const yMatch = gcodeLine.match(yRegex);
    const zMatch = gcodeLine.match(zRegex);
    const iMatch = gcodeLine.match(iRegex);
    const jMatch = gcodeLine.match(jRegex);
    const rMatch = gcodeLine.match(rRegex);
    

    let startX = x0;
    let startY = y0;
    let startZ = z0;
    let endX = x0;
    let endY = y0;
    let endZ = z0;
    let centerX = 0;
    let centerY = 0;
    let radius = 0;
    let clockwise = true;

    if (xMatch) {
        endX = parseFloat(xMatch[1]);
        // startX = x0;
    }

    if (yMatch) {
        endY = parseFloat(yMatch[1]);
        // startY = y0;
    }

    if (zMatch) {
        endZ = parseFloat(zMatch[1]);
        // startZ = z0;
    }

    const command = gcodeLine.split(' ')[0];
    let coordinates = [];
    console.log("command: " + command)
    if (command === 'G0' || command === 'G1') {
        const totalDistance = Math.sqrt(
        Math.pow(endX - startX, 2) + Math.pow(endY - startY, 2) + Math.pow(endZ - startZ, 2)    //Pitagoras en r3
        );
        const numSteps = Math.ceil(totalDistance / timeStep);

        for (let i = 0; i <= numSteps; i++) {
            const t = i / numSteps;
            const currentX = startX + t * (endX - startX);
            const currentY = startY + t * (endY - startY);
            const currentZ = startZ + t * (endZ - startZ);

            coordinates.push({ x: currentX, y: currentY, z: currentZ });
        }
    } else if (command === 'G2' || command === 'G3') {
      clockwise = command === 'G2';
      if (xMatch && yMatch && iMatch && jMatch) {
                endX = parseFloat(xMatch[1]);
                endY = parseFloat(yMatch[1]);
                endZ = parseFloat(zMatch ? zMatch[1] : startZ);
                centerX = startX + parseFloat(iMatch[1]);
                centerY = startY + parseFloat(jMatch[1]);
                radius = Math.sqrt(Math.pow(centerX - startX, 2) + Math.pow(centerY - startY, 2));
                coordinates = interpolateArc(startX, startY, startZ, endX, endY, endZ, centerX, centerY, radius, clockwise, timeStep);
              } else if (xMatch && yMatch && rMatch) {
                endX = parseFloat(xMatch[1]);
                endY = parseFloat(yMatch[1]);
                endZ = parseFloat(zMatch ? zMatch[1] : startZ);
                const radiusOffset = parseFloat(rMatch[1]);
                const currentX = startX + parseFloat(xMatch[1]);
                const currentY = startY + parseFloat(yMatch[1]);
                const currentZ = parseFloat(zMatch ? zMatch[1] : startZ);
            
                coordinates.push({ x: currentX, y: currentY, z: currentZ });
            
                const numSteps = Math.ceil((Math.PI * radiusOffset) / timeStep);
            
                for (let i = 1; i <= numSteps; i++) {
                  const t = i / numSteps;
                  const currentAngle = t * Math.PI;
                  const currentX = startX + radiusOffset * Math.cos(currentAngle);
                  const currentY = startY + radiusOffset * Math.sin(currentAngle);
                  const currentZ = parseFloat(zMatch ? zMatch[1] : startZ);
            
                  coordinates.push({ x: currentX, y: currentY, z: currentZ });
                }
        }
        
      }
      return coordinates;
}

export default animateGCode;



