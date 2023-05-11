import React, { useState, useEffect } from 'react';

function GcodeText(props) {

  return (
    <div className="w-10/12 h-fit max-h-96 bg-neutral shadow-xl m-3 rounded">
      <input type="file" className="file-input file-input-bordered file-input-error w-full rounded" onChange={props.handleFileInput} />
      <label htmlFor="my-modal" className="btn w-full gap-2 bg-base-100 rounded">G Code Console</label>
      <input type="checkbox" id="my-modal" className="modal-toggle" />
      <div className="modal">
        <div className="modal-box">
          {props.fileContent.split('\n').map((line, index) => (
            <div className='w-full flex hover:bg-neutral' key={index}>
              <span className='w-10 pl-5 pr-10 text-primary text-left'>{index + 1}</span>
              <code className='text-primary-content'>{line}</code>
            </div>
          ))}
          <div className="modal-action">
            <label className="btn" htmlFor="my-modal" >
              Continue
            </label>
          </div>
        </div>
      </div>
      <div className='h-fit max-h-60 overflow-auto'>
        {props.fileContent.split('\n').map((line, index) => (
          <div className='w-full flex hover:bg-sky-700' key={index}>
            <span className='w-10 pl-5 pr-10 text-base-100 text-left'>{index + 1}</span>
            <code className='text-primary-content'>{line}</code>
          </div>
        ))}
      </div>
      <div className='w-full h-10 flex mt-2'>
        <button className='tooltip w-10 pl-5 pr-10 text-succes text-left text-primary hover:text-accent-content hover:bg-primary rounded tooltip-secondary' data-tip="Add line">
          {">"}
        </button>
        <input className="border-sky-700 placeholder-slate-400 contrast-more:border-slate-400 contrast-more:placeholder-slate-500 flex-1" />
      </div>
      <button className='btn w-full gap-2 bg-warning text-warning-content rounded' onClick={props.handleSendGCode} disabled={!props.portStatus || !props.fileContent || props.sendingInProgress}>
        Send G-Code
      </button>
    </div>
  );
  
}

export default GcodeText