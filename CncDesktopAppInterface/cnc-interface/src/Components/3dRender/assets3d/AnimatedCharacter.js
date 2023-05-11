import React, { useRef } from "react";
import { useLoader, useFrame } from "@react-three/fiber";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader";

// export default function Model(props) {
//   const group = useRef();
//   const { nodes, materials, animations } = useGLTF(
//     "/animatedCharacters/punta.glb"
//   );
//   const { actions } = useAnimations(animations, group);
//   useEffect(() => {
//     actions.OverShoulder.play();
//   });
//   return (
//     <group ref={group} dispose={null}>
//       <group name="Scene">
//         <group
//           name="Armature"
//           rotation={[Math.PI / 2.2, 2.8, 0]}
//           scale={400}
//           position={[0, 0, -3.5]}
//         >
//           <primitive object={nodes.mixamorigHips} />
//           <skinnedMesh
//             name="unamed"
//             geometry={nodes.unamed.geometry}
//             material={materials.palettemat}
//             skeleton={nodes.unamed.skeleton}
//           />
//         </group>
//       </group>
//     </group>
//   );
// }

// useGLTF.preload("/animatedCharacters/animatedCharacter.glb");

const AnimatedCharacter = () => {
  const gltf = useLoader(GLTFLoader, "/animatedCharacters/punta.glb");
  const model = useRef();

  useFrame(() => {
    // Add any animation or manipulation logic here
    // Example: model.current.rotation.y += 0.01;
  });

  return <primitive object={gltf.scene} ref={model} />;
};

export default AnimatedCharacter;
