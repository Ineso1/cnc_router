import React from 'react'

function CncFunctionCard() {
  return (
    <div className="w-10/12 h-fill bg-neutral shadow-xl m-3 rounded">
        <div className="card-body">
            <h2>Step for functions</h2>
            <div className='grid grid-flow-col-3 py-2'>
                <div className='col-span-1 pr-1'>
                    <span>X</span>
                    <input type="text" placeholder="mm" className="input w-full max-w-xs rounded" />
                </div>
                <div className='col-span-1'>
                    <span>Y</span>
                    <input type="text" placeholder="mm" className="input w-full max-w-xs rounded" />
                </div>
                <div className='col-span-1 pl-1'>
                    <span>Z</span>
                    <input type="text" placeholder="mm" className="input w-full max-w-xs rounded" />
                </div>
                <div className='col-span-3'></div>
            </div>
            <div className='grid grid-flow-col-4'>
                <div class="col-span-1"></div>
                <div class="kbd col-span-1  hover:bg-sky-700 cursor-pointer">Y ▲</div>
                <div class="col-span-1"></div>
                <div class="kbd col-span-1 hover:bg-sky-700 cursor-pointer">◀︎ X</div>
                <div class="col-span-1"></div>
                <div class="kbd col-span-1 hover:bg-sky-700 cursor-pointer">X ▶︎</div>
                <div class="col-span-1"></div>
                <div class="kbd col-span-1 hover:bg-sky-700 cursor-pointer">Y ▼</div>
                <div class="col-span-1"></div>
                <div class="kbd col-span-1 hover:bg-sky-700 cursor-pointer">- Z</div>
                <div class=" col-span-1"></div>
                <div class="kbd col-span-1 hover:bg-sky-700 cursor-pointer">+ Z</div>
                <div class="kbd col-span-3 hover:bg-sky-700 cursor-pointer mt-5">Autoset</div>
            </div> 
        </div>
    </div>
  )
}

export default CncFunctionCard