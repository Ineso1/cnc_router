import React, { Suspense } from "react";
import Window3dScenes from "./Window3dScenes";
import { OrbitControls, Plane } from "@react-three/drei";
import "./Window3d.css";
import AnimatedCharacter from "./assets3d/AnimatedCharacter"


function Window3d(props) {
  let ratio = 0.1
  let x = props.x * ratio;
  let y = props.y * ratio;
  let z = props.z * ratio;
  return (
    <div className="Window3d">
      <Window3dScenes className="Window3dScene" cameraPos={[0, 5, -8]}>
        {/* <color attach="background" args={['#ffff']}/> */}
        <Suspense fallback={null}>
          <AnimatedCharacter x={x} y={y} z={z} />
        </Suspense>
        <Plane args={[25, 20, 30, 30]} rotation={[-Math.PI / 2, 0, 0]}>
          <meshBasicMaterial attach="material" wireframe />
        </Plane>
        <OrbitControls
          // autoRotate
          // autoRotateSpeed={2}
          minDistance={7}
          maxDistance={20}
          enablePan={false}
        />
        <ambientLight intensity={0.7} />
        <pointLight color="yellow" intensity={1} position={[10, 10, 20]} />
      </Window3dScenes>
    </div>
  );
}

export default Window3d;
