class Modification < ActiveRecord::Base
  belongs_to :renter
  belongs_to :host
  belongs_to :transaction
end
