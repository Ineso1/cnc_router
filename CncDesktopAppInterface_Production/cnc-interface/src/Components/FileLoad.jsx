import React, { useState, useEffect } from 'react';

function FileLoad(props) {


  return (
    <div className="w-10/12 h-fit bg-neutral shadow-xl m-3 flex flex-col rounded items-center pb-4">
        <div className='w-full flex items-center py-2'>
          <h3 className='px-5'>State</h3>        
          <spam className={`w-full badge ${props.portStatus ? 'badge-success' : 'badge-error'} gap-2 hover:cursor-pointer`} onClick={props.reconect}>
            {props.portStatus ? 'Connected' : 'Disconnected'}
          </spam>
        </div>
        <div className='w-full flex items-center'>
          <h3 className='px-5'>Name</h3>        
            <spam className="w-full gap-2 overflow-auto">
              {props.portName}
            </spam>
        </div>
        <div className='w-full flex items-center'>
          <h3 className='px-5'>-</h3>        
          <spam className="w-full badge gap-2">
            -
          </spam>
        </div>
        <div className='w-full flex items-center'>
          <h3 className='px-5'>-</h3>        
          <spam className="w-full badge gap-2">
            -
          </spam>
        </div>
    </div>
  )
}

export default FileLoad