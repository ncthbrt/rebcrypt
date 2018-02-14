module Salt: {
  type t; 
  let toString: t => string;
  let genSync: (~rounds: int=?, unit) => t;
  let gen: (~rounds: int=?, unit) => Js.Promise.t(t);
};

module Hash: {
  type t; 
  let toString: t => string;
  let getRounds: t => int;
  let makeSync: (string, Salt.t) => t;
  let make: (string, Salt.t) => Js.Promise.t(t);
  let compareSync: (string, t) => bool;
  let compare: (string, t) => Js.Promise.t(bool);
};

let hashAndSaltSync: (~rounds: int=?, string) => Hash.t;

let hashAndSalt: (~rounds: int=?, string) => Js.Promise.t(Hash.t);
