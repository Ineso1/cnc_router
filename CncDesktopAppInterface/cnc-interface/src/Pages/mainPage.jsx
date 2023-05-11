import React, { useState, useEffect } from 'react'
import Loader from '../Components/Loader';
import Header from '../Components/Header';
import CncFunctionCard from '../Components/CncFunctionCard';
import Values from '../Components/Values';
import FileLoad from '../Components/FileLoad';
import GcodeText from '../Components/GcodeText';
import Window3d from '../Components/3dRender/Window3d';

function MainPage() {
    const [loading, setLoading] = useState(false);
    useEffect(() => {
        console.log("cola");
        setLoading(true);
        setTimeout(() => {
            setLoading(false);
        }, 6000);
      }, []);
  return (
    <div>
      {loading ? (
        <div className='flex items-center justify-center flex-row h-screen w-screen'>
            <Loader/>
        </div>
      ) : (
        <>
          <div className='h-screen w-full'>
            <Header/>
            <div className='h-full w-full grid grid-cols-4'>
              <div className='flex h-full w-full pt-20 pb-5 justify-self-start z-10'>
                  <div className='grid grid-cols-1 gap-3 h-full w-full'>
                      <FileLoad/>
                      <GcodeText/>
                  </div>
              </div>
              <div className='flex h-full w-full pt-20 pb-5 col-span-2'>
                <div className='absolute h-full w-full left-0'>
                  <Window3d/>
                </div>
              </div>
              <div className='flex h-full w-full pt-20 pb-5 justify-self-end z-10'>
                  <div className='flex flex-col items-center h-full w-full col-start-5 col-span-1'>
                    <Values/>
                    <CncFunctionCard/>
                  </div>
              </div>
            </div>
          </div>
        </>
      )}
    </div>
  )
}

export default MainPage