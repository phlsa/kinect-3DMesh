//
//  Smoother.h
//  Kinect3D
//
//  Created by Philipp Sackl on 21.05.12.
//  Copyright (c) 2012 envis precisely GmbH. All rights reserved.
//

class Smoother {
public:
	int maxValues;
	std::vector<float> values;
	
	Smoother() {
		maxValues = 10;
		for ( int i = 0; i < maxValues; i++ ) {
			values.push_back( 0 );
		}
	}
	
	Smoother( float maxVals ) {
		maxValues = maxVals;
		for ( int i = 0; i < maxValues; i++ ) {
			values.push_back( 0 );
		}
	}
	
	void addValue( float theValue ) {
		values.erase( values.begin() );
		values.push_back( theValue );
	}
	
	float getValue() {
		float c = 0;
		for ( int i = 0; i < maxValues; i++ ) {
			c += values.at( i );
		}
		
		return c / maxValues;
	}
	
	float getLast() {
		return values.at( maxValues - 1 );
	}
};