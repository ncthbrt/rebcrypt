open Jest;

open Js.Promise;

open ExpectJs;

let toBeASalt = Expect.toMatchRe(Js.Re.fromString("^\\$2[ayb]\\$.+$"));

let toBeAHash = Expect.toMatchRe(Js.Re.fromString("^\\$2[ayb]\\$[0-9]{2}\\$[A-Za-z0-9\\.\\/]{53}$"));

let (>>) = (f, g, x) => x |> f |> g;

open Rebcrypt;

describe("Salt.genSync", () => {
  test("should be able to generate a salt with custom rounds", () =>
    expect(Salt.genSync(~rounds=15, ()) |> Salt.toString) |> toBeASalt
  );
  test("should be able to generate a salt with default rounds", () =>
    expect(Salt.genSync() |> Salt.toString) |> toBeASalt
  );
});

describe("Salt.gen", () => {
  testPromise("should be able to generate a salt with custom rounds", () =>
    Salt.gen(~rounds=15, ())
    |> then_(Salt.toString >> expect >> toBeASalt >> resolve)
  );
  testPromise("should be able to generate a salt with default rounds", () =>
    Salt.gen() |> then_(Salt.toString >> expect >> toBeASalt >> resolve)
  );
});

describe("Hash.makeSync", () => {
  let salt = Salt.genSync();
  test("should be able to hash a string", () =>
    expect(Hash.makeSync("REALLY_STRING_PASSWORD", salt) |> Hash.toString)
    |> toBeAHash
  );
});

describe("Hash.make", () => {
  let salt = Salt.genSync();
  testPromise("should be able to hash a string", () =>
    Hash.make("REALLY_STRING_PASSWORD", salt)
    |> then_ (Hash.toString >> expect >> toBeAHash >> resolve)
  );
});

describe("hashAndSalt", () => {
  testPromise(
      "should be able to hash a string with default rounds", 
      () =>  hashAndSalt("REALLY_STRING_PASSWORD") |> then_ (Hash.toString >> expect >> toBeAHash >> resolve)    
  );
  testPromise(
      "should be able to hash a string with custom rounds", 
      () =>  hashAndSalt(~rounds=5, "REALLY_STRING_PASSWORD") |> then_ (Hash.toString >> expect >> toBeAHash >> resolve)    
  );
});


describe("hashAndSaltSync", () => {
  test(
      "should be able to hash a string with default rounds", 
      () =>  expect(hashAndSaltSync("REALLY_STRING_PASSWORD") |> Hash.toString) |> toBeAHash
  );
  test(
      "should be able to hash a string with custom rounds", 
      () =>  expect(hashAndSaltSync(~rounds=5, "REALLY_STRING_PASSWORD") |> Hash.toString) |> toBeAHash
  );
});


describe("Hash.compare", () => {
  testPromise("Identical passwords should hash to the same value", () => {
    hashAndSalt("REALLY_STRING_PASSWORD")
    |> then_(h => Hash.compare("REALLY_STRING_PASSWORD", h))
    |> then_ (expect >> toBeTruthy >> resolve);
  });

  testPromise("Different passwords should hash to the different values", () => {
    hashAndSalt("REALLY_WEEK_PASSWORD")
    |> then_(h => Hash.compare("REALLY_STRING_PASSWORD", h))
    |> then_ (expect >> toBeFalsy >> resolve);
  });

  testPromise("Identical hashed with different number of rounds should hash to the same value", () => {
    hashAndSalt(~rounds=4, "REALLY_STRING_PASSWORD")
    |> then_(Hash.compare("REALLY_STRING_PASSWORD"))
    |> then_ (expect >> toBeTruthy >> resolve);
  });
});

describe("Hash.compareSync", () => {
  test("Identical passwords should hash to the same value", () => {
    hashAndSaltSync("REALLY_STRING_PASSWORD")
    |> Hash.compareSync("REALLY_STRING_PASSWORD")
    |> (expect >> toBeTruthy);
  });

  test("Different passwords should hash to the different values", () => {
    hashAndSaltSync("REALLY_WEEK_PASSWORD")
    |> Hash.compareSync("REALLY_STRING_PASSWORD")
    |> (expect >> toBeFalsy);
  });

  test("Identical hashed with different number of rounds should hash to the same value", () => {
    hashAndSaltSync(~rounds=4, "REALLY_STRING_PASSWORD")
    |> Hash.compareSync("REALLY_STRING_PASSWORD")
    |> (expect >> toBeTruthy);
  });
});


describe("getRounds", () => {
  test("Should get the correct number of rounds", () => {
    expect(Hash.getRounds(hashAndSaltSync(~rounds=4, "REALLY_STRING_PASSWORD"))) |> toEqual(4)
  })
});