/*
   Copyright (C) 2024 Mike Kipnis - DistributedATS
*/

package components

import (
	"database/sql"
	"fmt"
	"os"

	"github.com/golang-jwt/jwt"
)

type InvestorCredentials struct {
	InvestorSecret string
	InvestorName   string
}

func PopulateInvestorCredenital(investors_db string) map[string]InvestorCredentials {

	var investor_credentials_map = make(map[string]InvestorCredentials)

	db, err := sql.Open("sqlite3", investors_db)

	if err != nil {
		fmt.Println("Unable to open", investors_db)
		os.Exit(0)
	}

	row, err := db.Query("SELECT * FROM investor")
	defer row.Close()

	if row == nil {
		fmt.Println("Investor database is empty ... exiting")
		os.Exit(0)
	}

	for row.Next() { // Iterate and fetch the records from result cursor
		var jwt_token string
		var secret string

		row.Scan(&jwt_token, &secret)

		token, err := jwt.Parse(jwt_token, func(token *jwt.Token) (interface{}, error) {
			// Don't forget to validate the alg is what you expect:
			if _, ok := token.Method.(*jwt.SigningMethodHMAC); !ok {
				return nil, fmt.Errorf("Unexpected signing method: %v", token.Header["alg"])
			}

			return []byte(secret), nil
		})

		if claims, ok := token.Claims.(jwt.MapClaims); ok && token.Valid {

			data := claims["username"].(string)
			name := data
			fmt.Println(name)

			var investor_credentials InvestorCredentials
			investor_credentials.InvestorSecret = secret
			investor_credentials.InvestorName = name

			investor_credentials_map[jwt_token] = investor_credentials

		} else {
			fmt.Println(err)
		}

	}

	return investor_credentials_map
}
