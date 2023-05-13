import React from 'react'


function CncFunctionCard(props) {

    return (
        <div className="w-10/12 h-fill bg-neutral shadow-xl m-3 rounded">
          <div className="card-body">
            <h2>Step for functions</h2>
            <div className="grid grid-flow-col-3 py-2">
              <div className="col-span-1 pr-1">
                <span>X</span>
                <input
                  type="text"
                  placeholder="mm"
                  className="input w-full max-w-xs rounded"
                  value={props.xMagnitude}
                  onChange={(e) => props.setXMagnitude(e.target.value)}
                />
              </div>
              <div className="col-span-1">
                <span>Y</span>
                <input
                  type="text"
                  placeholder="mm"
                  className="input w-full max-w-xs rounded"
                  value={props.yMagnitude}
                  onChange={(e) => props.setYMagnitude(e.target.value)}
                />
              </div>
              <div className="col-span-1 pl-1">
                <span>Z</span>
                <input
                  type="text"
                  placeholder="mm"
                  className="input w-full max-w-xs rounded"
                  value={props.zMagnitude}
                  onChange={(e) => props.setZMagnitude(e.target.value)}
                />
              </div>
              <div className="col-span-3"></div>
            </div>
            <div className="grid grid-flow-col-4">
              <div className="col-span-1"></div>
              <button
                className="btn kbd col-span-1 hover:bg-sky-700 cursor-pointer"
                onClick={() => props.moveY(parseFloat(props.yMagnitude))}
              >
                Y ▲
              </button>
              <div className="col-span-1"></div>
              <button
                className="btn kbd col-span-1 hover:bg-sky-700 cursor-pointer"
                onClick={() => props.moveX(`-${parseFloat(props.xMagnitude)}`)}
              >
                ◀︎ X
              </button>
              <div className="col-span-1"></div>
              <button
                className="btn kbd col-span-1 hover:bg-sky-700 cursor-pointer"
                onClick={() => props.moveX(parseFloat(props.xMagnitude))}
              >
                X ▶︎
              </button>
              <div className="col-span-1"></div>
              <button
                className="btn kbd col-span-1 hover:bg-sky-700 cursor-pointer"
                onClick={() => props.moveY(`-${parseFloat(props.yMagnitude)}`)}
              >
                Y ▼
              </button>
              <div className="col-span-1"></div>
              <button
                className="btn kbd col-span-1 hover:bg-sky-700 cursor-pointer"
                onClick={() => props.moveZ(`-${parseFloat(props.zMagnitude)}`)}
              >
                - Z
              </button>
              <div className="col-span-1"></div>
              <button
                className="btn kbd col-span-1 hover:bg-sky-700 cursor-pointer"
                onClick={() => props.moveZ(parseFloat(props.zMagnitude))}
              >
                + Z
              </button>
              <button
                className="btn kbd col-span-3 hover:bg-sky-700 cursor-pointer mt-5"
                onClick={props.autoSetCoordinates}
              >
                Autoset
              </button>
            </div>
          </div>
        </div>
      );
}

export default CncFunctionCard