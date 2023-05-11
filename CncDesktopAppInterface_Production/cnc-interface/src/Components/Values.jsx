import React from 'react'

function Values(props) {
  let x = props.x;
  let y = props.y;
  let z = props.z;

  return (
    <div className="w-10/12 h-fill bg-neutral shadow-xl m-3 rounded">
        <div className="card-body">
            <div className='flex items-center'><progress className="progress progress-success w-full" value="70" max="100"></progress><span className='stat-title font-bold'>70%</span></div>
            <div className='flex'><h2 className='stat-title mr-5 font-bold'>Time</h2>00:00:00<h4 className='ml-2'>s</h4></div>
            <div className='flex'><h2 className='stat-value mr-5 font-bold'>X</h2><h3 className='stat-value mr-2'>{x}</h3><h4>mm</h4></div>
            <div className='flex'><h2 className='stat-value mr-5 font-bold'>Y</h2><h3 className='stat-value mr-2'>{y}</h3><h4>mm</h4></div>
            <div className='flex'><h2 className='stat-value mr-5 font-bold'>Z</h2><h3 className='stat-value mr-2'>{z}</h3><h4>mm</h4></div>
        </div>
    </div>
  )
}
export default Values