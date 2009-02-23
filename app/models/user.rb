class User < ActiveRecord::Base
  has_one :renter
  has_one :host
  has_one :profile
end
