
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 * @providesModule ActivityIndicator
 * @flow
 */

'use strict';

const React = require('React');
var ReactNative = require('react-native');
var PropTypes = require('react/lib/ReactPropTypes');
var requireNativeComponent = require('requireNativeComponent');
const View = require('View');
const StyleSheet = require('StyleSheet');


var UbuntuActivityIndicator = requireNativeComponent('UbuntuActivityIndicator', null);

var ActivityIndicatorUbuntu = React.createClass({
  propTypes: {
    ...View.propTypes,
    animating: PropTypes.bool,
    size: PropTypes.oneOf([
      'small',
      'large',
    ]),
  },

  getDefaultProps: function() {
    return {
      animating: true,
      size: 'small',
    };
  },

  render: function() {
    var { animating, size } = this.props;
    return (
      <UbuntuActivityIndicator
        style={size === 'large' ? styles.sizeLarge : styles.sizeSmall}
        running={animating}>
      </UbuntuActivityIndicator>
    );
  }
});

var styles = StyleSheet.create({
  sizeSmall: {
    width: 20,
    height: 20,
  },
  sizeLarge: {
    width: 36,
    height: 36,
  }
});

module.exports = ActivityIndicatorUbuntu;
